/* INCLUDES */
#include "dapqmlmodelroutingexceptions.h"

#include <QList>
#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>

#include "DapDataLocal.h"

/* DEFS */

//#define TEST_MODE

typedef DapQmlModelRoutingExceptions::App App;
typedef DapQmlModelRoutingExceptions::Route Route;
typedef DapQmlModelRoutingExceptions::Field Field;
typedef DapQmlModelRoutingExceptions::Mode Mode;
typedef DapQmlModelRoutingExceptions::AppsContainer AppsContainer;

// base class with refresh method added

class RefreshingListModelBase : public QAbstractListModel
{
public:
  explicit RefreshingListModelBase (QObject *parent = nullptr);
  ~RefreshingListModelBase();
  void refresh();
};

// base class with model storage added

class RefreshingListModel : public RefreshingListModelBase
{
public:
  explicit RefreshingListModel (QObject *parent = nullptr);
  ~RefreshingListModel();
};

// apps model

class DqmreApps : public RefreshingListModelBase
{
public:
  DqmreApps();
  ~DqmreApps();
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

// routes model

class DqmreRoutes : public RefreshingListModel
{
public:
  DqmreRoutes();
  ~DqmreRoutes();
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

// checked apps model

class DqmreCheckedApps : public RefreshingListModel
{
  AppsContainer *_container;
public:
  DqmreCheckedApps (AppsContainer *a_container);
  ~DqmreCheckedApps();
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

// sorted apps model

class DqmreSortedApps : public RefreshingListModel
{
  AppsContainer *_container;
public:
  DqmreSortedApps (AppsContainer *a_container);
  ~DqmreSortedApps();
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

// icon container

class AppIcon : public QByteArray
{
public:
  AppIcon() : QByteArray() {}
  AppIcon (const QByteArray &a_src) : QByteArray (a_src) {}
  AppIcon (QByteArray &&a_src) : QByteArray (std::move (a_src)) {}
  AppIcon (const QString &a_src) : QByteArray (a_src.toUtf8()) {}
  //AppIcon (QString &&a_src) : QByteArray (std::move (a_src.toUtf8())) {}
  AppIcon (const AppIcon &a_src) : QByteArray (a_src) {}
  AppIcon (AppIcon &&a_src) : QByteArray (std::move (a_src)) {}
#ifdef Q_OS_ANDROID
  operator QImage() const;
#endif // Q_OS_ANDROID
  AppIcon &operator = (const QByteArray &a_src) { QByteArray::operator = (a_src); return *this; }
  AppIcon &operator = (const QString &a_src)    { QByteArray::operator = (a_src.toUtf8()); return *this; }
  AppIcon &operator = (const AppIcon &a_src)    { QByteArray::operator = (a_src); return *this; }
  AppIcon &operator = (QByteArray &&a_src) { QByteArray::operator = (std::move (a_src)); return *this; }
  //AppIcon &operator = (QString &&a_src)    { QByteArray::operator = (std::move (a_src.toUtf8())); return *this; }
  AppIcon &operator = (AppIcon &&a_src)    { QByteArray::operator = (std::move (a_src)); return *this; }
};

/* VARS */

//static const QString SETTING_ROUTING_EXCEPTIONS {"RoutingExceptions"};

// all apps list
static QList<App> s_apps;
// all routes list
static QList<Route> s_routes;
// apps icons map
static QMap<QString, AppIcon> s_iconMap;
//// only checked apps
//static QList<App> s_checkedApps;
//// only unchecked apps
//static QList<App> s_uncheckedApps;
//// sorted apps : checked at top, unchecked at the bottom
//static QList<App> s_sortedApps;
static AppsContainer s_excluded {Mode::EXC_CHECKED_APPS, {}, {}, {}},
                     s_included {Mode::INC_CHECKED_APPS, {}, {}, {}};

// models to update
static QList<RefreshingListModel *>     s_models;
static QList<RefreshingListModelBase *> s_baseModels;

static const QHash<int, QByteArray> s_appsRoles =
{
  {Field::packageName,  "packageName"},
  {Field::appName,      "appName"},
  {Field::icon,         "icon"},
  {Field::checked,      "checked"},
};

static const QHash<int, QByteArray> s_routesRoles =
{
  {Field::address,      "address"},
  {Field::description,  "description"},
};

static const QHash<Field, QByteArray> s_allRoles =
{
  {Field::packageName,  "packageName"},
  {Field::appName,      "appName"},
  {Field::icon,         "icon"},
  {Field::checked,      "checked"},
  {Field::address,      "address"},
  {Field::description,  "description"},
};

#ifdef TEST_MODE

struct DummyApp
{
  QString pname, aname, icon;
  bool checked;
};

static const QList<DummyApp> s_dummyApps =
{
  {
    "Telegram.pak",
    "Telegram",
    "iVBORw0KGgoAAAANSUhEUgAAAGAAAABgCAYAAADimHc4AAAACXBIWXMAACE4AAAhOAFFljFgAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAV0SURBVHgB7Z2/bxtlGMe/54SmlYrqKqgViII7ABIiaqwGFpAa1A0GwsgU5y9IMqA0WepOSVjqbmyxJzYoS7cqiQSqBK7iqIhKVAiHIlCCkC4CQYJaH+/3zLVuYjt27u59avv5SG6c87lWnu/z6318p9dBK2TXkhh4MAzPGYODC+aRMkeTUGpxzaOECtbNzxXMjVxv5U1O01dp+KOVScCbghq8TbwykMjDSxQwmy43OquxAJ8UJ42aWajhQ2KEcPqzmEkX6r1aX4DF21fh+V6vRIXj5DBzfnrf4X0nLhSXzL8ZKDHg5HHp/ETtkcQTr9Pz1fgx4mX+t/EjHkfAfDFjfluCEj+ON42ZN3P+U//A/K0UnCPLRqEUFBu42Ok7i2zaraYg58i4Gt8qXFf5TU41AhZu/6QCWMePggQWi2NqfBH86UICHkahyGBGO6wB56DI0IcLFGAYigweUhRAZz1yJBNQRFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhOlHj5Ic6MPwqWMYH3oOoy8dR+rEAMrb/2LiRhkrP/8JWzhYKHroIUbPHMfYqycx/sYgkkf79r1OEc5+ege26IkIoLePDw1i7JWk8fZnm56bOnEENulqAQ7y9qeBrhOgHW+vB1OQTbpGABbUjCmoYb29vL0Lm3S8AEwzl9954VDeXo/t3YewSUcKwDQzOXIKUyOnI8/tpc2/YZOOEsBGUV37/R/YpCMEiDrNNGN7R1OQT9g0w2I6ffMXXL/nYi3zOoZPH2vpfT2fgqJIM9eKW8h+9StcU1DZHbVqfJ7v9moRjiLN0Osnbmw8muXQ+Msfvdby+217PxEVIEgz7N/DjgBqvZ4Exm8nimy3oEREABqbKSaKNnKv15PDGJ90fQRE3c3s9XpyWOMT2y0oiV2AsLOZetTzesKoyl08c+io2nDtjiFIbALEtVqt5/WExs+/n0IYSltdEgFhPbEejbyeUGh+XhhKW/bzP4lFgKzJ8za8nlx++3n/88KyYXkMHRCLAPSmKL5Zaub1JCrjB58lQSxXRXz4+Y+hv9ym16eX7jb8P65efDEy45Pl+39BgtiKcP7OH/6DkcDu5wPTBbETOoiDvJ4svfeyv3iLEttDuADrV0VQDApxzvTrtW0pDb5qvDD37WbTeUwcxicncyXrcyBifSVMQ9d6N6/HcXcetPTHx2V81iwJ4xPxYVyrxY85Pw7jE6kOiHTEpYlc1HFBFxcSM6AAvTYUMjOggI4QgPn5yte/IS4kZkABHRMBXAlP37xvCnb0xVJiBhTQUSkox8VZ/vtIr16WmgEFdFwN4KWD7372Q2TRINkBkY4twkE0hO1gJDsg0tFdEKMhnb8bqkCvCM2AArqiDWWB5k0Vh5loagRERPXOlu/aigaJ64D20nULsXaiYdXivWCN6MqVcKvR8MU9F9J09SiiWTTwWMF8XyFNz9wlmRka9L8U4mBv3ax8c8VN67cj1aPnblN92tBpqDAqgDAqgDAqgDAqgDAqgDAqgDAqgDAqgDAUQH4m27u43EesDEWKUsLMQ1ehyFDBuokAr6XN55UYeKZvOYHd/hK0DgjglfFx+ssE97Q1UXANil08FPij2obu9nN3Z40CaxjvR3+ez6oCMAoq3hUodnD6s5hNl/n08UJszt/jXFNR3FSMjWfSheBXZ98J89/k4STGoUSPVylg9q1M7aH9o4jqCRoJUUPP32N8Un8WdGlkylTpCRMgZShhceF405gzNq2D0/St87dSplpnqilJ951vk2p7zw6TTU4DHLTKYnEMDzFqYoZboHMXbt0I+klcf67mmdGOY6YLXOA2MXzAf1OiLlNAMR3NAAAAAElFTkSuQmCC",
    false,
  },
  {
    "WhatsApp.pak",
    "WhatsApp",
    "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",
    true,
  },
  {
    "Discord.pak",
    "Discord",
    "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",
    false,
  },
};

#endif // TEST_MODE

/* FUNCS */

static App toApp (const QJsonObject &a_src);
static App toApp (const QVariantMap &a_src);
static QString getIcon (const QJsonObject &a_src);
static QString getIcon (const QVariantMap &a_src);
static Route toRoute (const QJsonObject &a_src);
static Route toRoute (const QVariantMap &a_src);
static QJsonObject toJson (const App &a_src);
static QJsonObject toJson (const Route &a_src);
#ifdef Q_OS_ANDROID
static QImage getAppIcon (const QString &a_packageName);
#endif // Q_OS_ANDROID

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelRoutingExceptions::DapQmlModelRoutingExceptions()
  : m_mode (NONE)
  , _model (nullptr)
  , _container (nullptr)
{

}

/********************************************
 * METHODS
 *******************************************/

int DapQmlModelRoutingExceptions::mode() const
{
  return m_mode;
}

void DapQmlModelRoutingExceptions::setMode (int a_newMode)
{
  /* checks */
  if (m_mode == a_newMode
      || a_newMode >= _SIZE)
    return;

  beginResetModel();

  /* store mode */
  m_mode  = Mode (a_newMode);

  /* delete model, if present */
  if (_model)
    delete _model;

  /* clear */
  _model      = nullptr;
  _container  = nullptr;

  /* set container */
  switch (m_mode)
  {
    case EXC_CHECKED_APPS:
    case EXC_SORTED_APPS:
      _container  = &s_excluded;
      break;
    case INC_CHECKED_APPS:
    case INC_SORTED_APPS:
      _container  = &s_included;
      break;
    default:
      break;
  }

  /* set new model based on new mode */
  switch (m_mode)
    {
    case NONE: return;
    case EXC_CHECKED_APPS:
    case INC_CHECKED_APPS:
      _model  = new DqmreCheckedApps (_container);
      break;
    case EXC_SORTED_APPS:
    case INC_SORTED_APPS:
      _model  = new DqmreSortedApps  (_container);
      break;
    case ALL_APPS:          _model  = new DqmreApps;        break;
    case ALL_ROUTES:        _model  = new DqmreRoutes;      break;
    default: break;
    }

  /* signals */
  connect (_model, &QAbstractListModel::modelAboutToBeReset,
           this, &QAbstractListModel::modelAboutToBeReset);
  connect (_model, &QAbstractListModel::modelReset,
           this, &QAbstractListModel::modelReset);

#ifdef TEST_MODE
  if (s_apps.isEmpty())
    {
      for (const auto &dummyApp : s_dummyApps)
        {
//          QImage icn;
//          QByteArray icnSrc (dummyApp.icon.toUtf8());
//          icn.loadFromData (QByteArray::fromBase64 (icnSrc));
          append (App {dummyApp.pname, dummyApp.aname, dummyApp.checked}, std::move (dummyApp.icon));
        }
//      append (App{"TestApp1", "qrc:/nonthemed/conn-icon-3.png", false, QImage()});
//      append (App{"TestApp2", "qrc:/nonthemed/conn-icon-4.png", true,  QImage()});
//      append (App{"TestApp3", "qrc:/nonthemed/conn-icon-5.png", false, QImage()});
      updateAllLists();
    }

  if (s_routes.isEmpty())
    {
      append (Route{"TestRoute1", "127.0.0.1"});
      append (Route{"TestRoute2", "127.0.0.2"});
      append (Route{"TestRoute3", "127.0.0.3"});
      append (Route{"TestRoute4", "127.0.0.4"});
      updateAllLists();
    }
#endif // TEST_MODE

  endResetModel();

  emit sigModeChanged();
}

void DapQmlModelRoutingExceptions::append (const DapQmlModelRoutingExceptions::App &a_app, const QString &a_icon)
{
  beginInsertRows (QModelIndex(), s_apps.size(), s_apps.size());
  s_iconMap[a_app.packageName + ".png"] = AppIcon (a_icon);
  s_apps.append (a_app);
  endInsertRows();

  _appendCheckedApp (s_apps.size() - 1);
}

void DapQmlModelRoutingExceptions::append (DapQmlModelRoutingExceptions::App &&a_app, QString &&a_icon)
{
  beginInsertRows (QModelIndex(), s_apps.size(), s_apps.size());
  s_iconMap[a_app.packageName + ".png"] = std::move (a_icon);
  s_apps.append (std::move (a_app));
  endInsertRows();

  _appendCheckedApp (s_apps.size() - 1);
}

void DapQmlModelRoutingExceptions::append (const DapQmlModelRoutingExceptions::Route &a_route)
{
  beginInsertRows (QModelIndex(), s_routes.size(), s_routes.size());
  s_routes.append (a_route);
  endInsertRows();
}

void DapQmlModelRoutingExceptions::append (DapQmlModelRoutingExceptions::Route &&a_route)
{
  beginInsertRows (QModelIndex(), s_routes.size(), s_routes.size());
  s_routes.append (std::move (a_route));
  endInsertRows();
}

void DapQmlModelRoutingExceptions::appendJson (const QVariant &a_value)
{
  /* get json */
  QVariantMap vmap  = a_value.toMap();
  //QJsonObject jobj  = a_value.toJsonObject();

  /* decide which mode this json refer to */
  Mode _mode  = NONE;
  if (vmap.contains ("name"))
    _mode = ALL_APPS;
  else if (vmap.contains ("address"))
    _mode = ALL_ROUTES;

  /* act to mode */
  switch (_mode)
    {
    case NONE: return;
    case ALL_APPS:    append (toApp (vmap), getIcon (vmap));   break;
    case ALL_ROUTES:  append (toRoute (vmap)); break;
    default: break;
    }
}

void DapQmlModelRoutingExceptions::appendQuietly (DapQmlModelRoutingExceptions::App &&a_app, QString &&a_icon)
{
  s_iconMap[a_app.packageName + ".png"] = std::move (a_icon);
  s_apps.append (std::move (a_app));
}

void DapQmlModelRoutingExceptions::insert (int a_index, const DapQmlModelRoutingExceptions::App &a_app, const QString &a_icon)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  s_iconMap[a_app.packageName + ".png"] = a_icon;
  s_apps.insert (a_index, a_app);
  endInsertRows();

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::insert (int a_index, DapQmlModelRoutingExceptions::App &&a_app, QString &&a_icon)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  s_iconMap[a_app.packageName + ".png"] = std::move (a_icon);
  s_apps.insert (a_index, std::move (a_app));
  endInsertRows();

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::insert (int a_index, const DapQmlModelRoutingExceptions::Route &a_route)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  s_routes.insert (a_index, a_route);
  endInsertRows();
}

void DapQmlModelRoutingExceptions::insert (int a_index, DapQmlModelRoutingExceptions::Route &&a_route)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  s_routes.insert (a_index, std::move (a_route));
  endInsertRows();
}

void DapQmlModelRoutingExceptions::insertJson (int a_index, const QVariant &a_value)
{
  /* get json */
  QVariantMap vmap  = a_value.toMap();
  //QJsonObject jobj  = a_value.toJsonObject();

  /* decide which mode this json refer to */
  Mode _mode  = NONE;
  if (vmap.contains ("name"))
    _mode = ALL_APPS;
  else if (vmap.contains ("address"))
    _mode = ALL_ROUTES;

  /* act to mode */
  switch (_mode)
    {
    case NONE: return;
    case ALL_APPS:    insert (a_index, toApp (vmap), getIcon (vmap));   break;
    case ALL_ROUTES:  insert (a_index, toRoute (vmap)); break;
    default: break;
    }
}

void DapQmlModelRoutingExceptions::replace (int a_index, const DapQmlModelRoutingExceptions::App &a_app, const QString &a_icon)
{
  if (!a_icon.isNull())
    s_iconMap[a_app.packageName + ".png"] = a_icon;
  s_apps[a_index] = a_app;
  auto changedIndex = index (a_index);
  emit dataChanged (changedIndex, changedIndex);

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::replace (int a_index, DapQmlModelRoutingExceptions::App &&a_app, const QString &a_icon)
{
  if (!a_icon.isNull())
    s_iconMap[a_app.packageName + ".png"] = a_icon;
  s_apps[a_index] = std::move (a_app);
  auto changedIndex = index (a_index);
  emit dataChanged (changedIndex, changedIndex);

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::replaceSorted (int a_index, const DapQmlModelRoutingExceptions::App &a_app, const QString &a_icon)
{
  if (_container == nullptr)
    return;

  int index = 0;
  for (auto i = s_apps.begin(), e = s_apps.end(); i != e; i++, index++)
    {
      if (i->packageName == a_app.packageName)
        {
          *i = a_app;
          break;
        }
    }

  if (index >= s_apps.size())
    return;

  if (!a_icon.isNull())
    s_iconMap[a_app.packageName + ".png"] = a_icon;
  _container->sortedApps[a_index] = a_app;
  auto changedIndex = this->index (a_index);
  emit dataChanged (changedIndex, changedIndex);
}

void DapQmlModelRoutingExceptions::replaceSorted (int a_index, DapQmlModelRoutingExceptions::App &&a_app, const QString &a_icon)
{
  if (_container == nullptr)
    return;

  int index = 0;
  for (auto i = s_apps.begin(), e = s_apps.end(); i != e; i++, index++)
    {
      if (i->packageName == a_app.packageName)
        {
          *i = a_app;
          break;
        }
    }

  if (index >= s_apps.size())
    return;

  if (!a_icon.isNull())
    s_iconMap[a_app.packageName + ".png"] = a_icon;
  _container->sortedApps[a_index] = std::move (a_app);
  auto changedIndex = this->index (a_index);
  emit dataChanged (changedIndex, changedIndex);
}

void DapQmlModelRoutingExceptions::replace (int a_index, const DapQmlModelRoutingExceptions::Route &a_route)
{
  s_routes[a_index] = a_route;
  auto changedIndex = index (a_index);
  emit dataChanged (changedIndex, changedIndex);
}

void DapQmlModelRoutingExceptions::replace (int a_index, DapQmlModelRoutingExceptions::Route &&a_route)
{
  s_routes[a_index] = std::move (a_route);
  auto changedIndex = index (a_index);
  emit dataChanged (changedIndex, changedIndex);
}

void DapQmlModelRoutingExceptions::replaceJson (int a_index, const QVariant &a_value)
{
  /* get json */
  QVariantMap vmap  = a_value.toMap();
  //QJsonObject jobj  = a_value.toJsonObject();

  /* decide which mode this json refer to */
  Mode _mode  = NONE;
  if (vmap.contains ("name"))
    _mode = ALL_APPS;
  else if (vmap.contains ("address"))
    _mode = ALL_ROUTES;

  /* act to mode */
  switch (_mode)
    {
    case NONE: return;
    case ALL_APPS:    replace (a_index, toApp (vmap));   break;
    case ALL_ROUTES:  replace (a_index, toRoute (vmap)); break;
    default: break;
    }
}

const DapQmlModelRoutingExceptions::App &DapQmlModelRoutingExceptions::app (int a_index) const
{
  return s_apps.at (a_index);
}

const DapQmlModelRoutingExceptions::App &DapQmlModelRoutingExceptions::appSorted (int a_index) const
{
  static App dummy;
  return (_container)
         ? _container->sortedApps.at (a_index)
         : dummy;
}

#ifdef Q_OS_ANDROID
const QImage &DapQmlModelRoutingExceptions::appIcon (const QString &a_packageName) const
{
  static QImage result;
  result  = getAppIcon (a_packageName);
  return result;
}
#endif // Q_OS_ANDROID

const DapQmlModelRoutingExceptions::Route &DapQmlModelRoutingExceptions::route (int a_index) const
{
  return s_routes.at (a_index);
}

QVariant DapQmlModelRoutingExceptions::appJson (int a_index) const
{
  return toJson (s_apps.at (a_index));
}

QVariant DapQmlModelRoutingExceptions::routeJson (int a_index) const
{
  return toJson (s_routes.at (a_index));
}

int DapQmlModelRoutingExceptions::indexOfChecked (int a_index) const
{
  if (_container == nullptr)
    return -1;

  App &app  = _container->checkedApps[a_index];
  int result = 0;
  for (auto i = s_apps.cbegin(), e = s_apps.cend(); i != e; i++, result++)
    if (i->packageName == app.packageName)
      return result;
  return -1;
}

int DapQmlModelRoutingExceptions::indexOfSorted (int a_index) const
{
  if (_container == nullptr)
    return -1;

  App &app  = _container->sortedApps[a_index];
  int result = 0;
  for (auto i = s_apps.cbegin(), e = s_apps.cend(); i != e; i++, result++)
    if (i->packageName == app.packageName)
      return result;
  return -1;
}

int DapQmlModelRoutingExceptions::appSize() const
{
  return s_apps.size();
}

int DapQmlModelRoutingExceptions::routeSize() const
{
  return s_routes.size();
}

void DapQmlModelRoutingExceptions::removeApp (int a_index)
{
  beginRemoveRows (QModelIndex(), a_index, a_index);
  s_iconMap.remove (s_apps.at (a_index).packageName + ".png");
  s_apps.removeAt (a_index);
  endRemoveRows();

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::removeAppFromChecked (int a_index)
{
  int index = indexOfChecked (a_index);

  beginRemoveRows (QModelIndex(), a_index, a_index);
  App &app      = s_apps[index];
  if (isExcludedList())
    app.checked.excluded  = false;
  else
    app.checked.included  = false;
  s_apps[index] = app;
  endRemoveRows();

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::removeRoute (int a_index)
{
  beginRemoveRows (QModelIndex(), a_index, a_index);
  s_routes.removeAt (a_index);
  endRemoveRows();
}

void DapQmlModelRoutingExceptions::moveApp (int a_from, int a_to)
{
  QModelIndex dummy;
  beginMoveRows (dummy, a_from, a_from, dummy, a_to);
  s_apps.move (a_from, a_to);
  endMoveRows();
}

void DapQmlModelRoutingExceptions::moveRoute (int a_from, int a_to)
{
  QModelIndex dummy;
  beginMoveRows (dummy, a_from, a_from, dummy, a_to);
  s_routes.move (a_from, a_to);
  endMoveRows();
}

void DapQmlModelRoutingExceptions::sortAndUpdateAllLists()
{
  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::updateAllLists()
{
  for (auto *model : qAsConst (s_models))
    model->refresh();
}

void DapQmlModelRoutingExceptions::save() const
{
  /* var */
  QJsonArray jExApps, jInApps, jroutes;

  /* collect checked apps and their content */
  for (const auto &app : qAsConst (s_excluded.checkedApps))
    jExApps << toJson (app);
  for (const auto &app : qAsConst (s_included.checkedApps))
    jInApps << toJson (app);

  /* collect routes */
  for (const auto &route : qAsConst (s_routes))
    jroutes << toJson (route);

  /* store */
  QJsonObject jobj =
  {
    {"exApps", jExApps},
    {"inApps", jInApps},
    {"routes", jroutes},
  };

  DapDataLocal::instance()->saveSetting (DapBaseDataLocal::ROUTING_EXCEPTIONS_LIST, jobj);

  /* clear data */
  while (jExApps.erase (jExApps.begin()) != jExApps.end());
  while (jInApps.erase (jInApps.begin()) != jInApps.end());
  while (jroutes.erase (jroutes.begin()) != jroutes.end());
}

void DapQmlModelRoutingExceptions::load()
{
  /* defs */
  struct AppBlob
  {
    App app;
    AppIcon icon;
  };

  /* vars */
  QJsonArray jExApps, jInApps, jroutes;
  // used to easily search app of checked app
  QMap<QString, AppBlob> exAppMap;
  QMap<QString, AppBlob> inAppMap;
  int index = 0;

  /* fill */
  {
    auto jobj = DapDataLocal::instance()->getSetting (DapBaseDataLocal::ROUTING_EXCEPTIONS_LIST).toJsonObject();
    bool old  = jobj.contains ("apps");
    jExApps   = (!old) ? jobj.value ("exApps").toArray() : jobj.value ("apps").toArray();
    jInApps   = (!old) ? jobj.value ("inApps").toArray() : QJsonArray();
    jroutes   = jobj.value ("routes").toArray();
  }

  /* prepare to fill */
  _clearBeforeLoad();

  /* ----------------------------------------
   * collect app names
   * ------------------------------------- */

  auto collectAppNames = [] (const QJsonArray &a_apps, QMap<QString, AppBlob> &a_map)
  {
    for (const auto &japp : a_apps)
      {
        auto jobj = japp.toObject();
        auto app  = toApp (jobj);
        auto key  = app.packageName;
        a_map.insert (
          key,
          AppBlob
          {
            std::move (app),
            jobj.value ("icon").toString()
          });
      }
  };

  collectAppNames (jExApps, exAppMap);
  collectAppNames (jInApps, inAppMap);

  /* ----------------------------------------
   * check existing apps
   * ------------------------------------- */

  auto checkout = [] (QMap<QString, AppBlob> &a_appMap, App &a_app, bool excluded)
  {
    if (!a_appMap.contains (a_app.packageName))
      return;

    if (excluded)
      a_app.checked.excluded = true;
    else
      a_app.checked.included = true;
    a_appMap.remove (a_app.packageName);
  };

  for (auto j = s_apps.begin(), k = s_apps.end(); j != k; j++, index++)
    {
      auto &item  = *j;
      checkout (exAppMap, item, true);
      checkout (inAppMap, item, false);
      _appendCheckedApp (index, false);
    }

  /* ----------------------------------------
   * add missing apps
   * ------------------------------------- */

  auto addMissingApps = [this] (AppBlob &a_blob, bool excluded)
  {
    a_blob.app.checked.excluded = (excluded) ? true  : false;
    a_blob.app.checked.included = (excluded) ? false : true;

    appendQuietly (std::move (a_blob.app),
                   std::move (a_blob.icon));

    _appendCheckedApp (s_apps.size() - 1, false);
  };

  for (auto &blob : exAppMap)
    addMissingApps (blob, true);
  for (auto &blob : inAppMap)
    addMissingApps (blob, false);

  /* ----------------------------------------
   * load routes
   * ------------------------------------- */

  for (const auto &route : qAsConst (jroutes))
    append (toRoute (route.toObject()));

  /* ----------------------------------------
   * combine new list
   * ------------------------------------- */

  auto combineNewList = [] (AppsContainer *a_container)
  {
    a_container->sortedApps =
      a_container->checkedApps
      + a_container->uncheckedApps;
  };

  combineNewList (&s_excluded);
  combineNewList (&s_included);

  /* clear data */
  while (jExApps.erase (jExApps.begin()) != jExApps.end());
  while (jInApps.erase (jInApps.begin()) != jInApps.end());
  while (jroutes.erase (jroutes.begin()) != jroutes.end());
}

void DapQmlModelRoutingExceptions::clear()
{
  clearApps();
  clearRoutes();
}

void DapQmlModelRoutingExceptions::clearApps()
{
  s_apps.clear();
  s_iconMap.clear();

  s_excluded.checkedApps.clear();
  s_excluded.uncheckedApps.clear();
  s_excluded.sortedApps.clear();

  s_included.checkedApps.clear();
  s_included.uncheckedApps.clear();
  s_included.sortedApps.clear();

  updateAllLists();

  for (auto *model : qAsConst (s_baseModels))
    model->refresh();
}

void DapQmlModelRoutingExceptions::clearRoutes()
{
  s_routes.clear();
  updateAllLists();
}

QStringList DapQmlModelRoutingExceptions::getIncludedCheckedPackageList()
{
  QStringList _list;
  for (const auto &item : qAsConst(s_included.checkedApps))
    _list.push_front(item.packageName);
  return _list;
}

QStringList DapQmlModelRoutingExceptions::getExcludedCheckedPackageList()
{
  QStringList _list;
  for (const auto &item : qAsConst(s_excluded.checkedApps))
    _list.push_front(item.packageName);
  return _list;
}

bool DapQmlModelRoutingExceptions::isExcludedList() const
{
  switch (m_mode)
  {
  case EXC_CHECKED_APPS:
  case EXC_SORTED_APPS:
    return true;
  default:
    return false;
  }
}

bool DapQmlModelRoutingExceptions::isIncludedList() const
{
  switch (m_mode)
  {
  case INC_CHECKED_APPS:
  case INC_SORTED_APPS:
    return true;
  default:
    return false;
  }
}

bool DapQmlModelRoutingExceptions::isTestMode()
{
#ifdef TEST_MODE
  return true;
#else // TEST_MODE
  return false;
#endif // TEST_MODE
}

static void removeFrom (QList<App> &a_list, const App &a_app)
{
  for (auto i = a_list.begin(), e = a_list.end(); i != e; i++)
    {
      if (i->packageName != a_app.packageName)
        continue;
      a_list.erase (i);
      return;
    }
};

void DapQmlModelRoutingExceptions::_appendCheckedApp (int a_index, bool a_combine)
{
  /* variables */
  auto &app   = s_apps.at (a_index);
  auto store  = [] (AppsContainer *a_container, const App &a_app, bool checked, bool a_combine)
  {
    /* remove from old list */
    removeFrom (a_container->uncheckedApps, a_app);
    removeFrom (a_container->checkedApps,   a_app);

    /* append to desired list */
    if (checked)
      a_container->checkedApps.append (a_app);
    else
      a_container->uncheckedApps.append (a_app);

    /* combine new list */
    if (a_combine)
      {
        a_container->sortedApps =
          a_container->checkedApps
          + a_container->uncheckedApps;
      }
  };

  /* perform */
  store (&s_excluded, app, app.checked.excluded, a_combine);
  store (&s_included, app, app.checked.included, a_combine);

  /* update combined lists */
  if (a_combine)
    updateAllLists();
}

//void DapQmlModelRoutingExceptions::_removeCheckedApp (int a_index)
//{
//  /* variables */
//  auto &app     = s_apps.at (a_index);
//  bool checked  = (isExcludedList())
//                  ? app.checked.excluded
//                  : app.checked.included;

//  /* remove from old list */
//  if (checked)
//    removeFrom (_container->uncheckedApps, app);
//  else
//    removeFrom (_container->checkedApps, app);
//}

void DapQmlModelRoutingExceptions::_sortCheckedApps()
{
  /* reinsert all items */
  for (int i = 0; i < s_apps.size(); i++)
    _appendCheckedApp (i, false);

  /* combine new list */
  auto combine = [] (AppsContainer *a_container)
  {
    a_container->sortedApps =
      a_container->checkedApps
      + a_container->uncheckedApps;
  };

  combine (&s_excluded);
  combine (&s_included);

  updateAllLists();
}

void DapQmlModelRoutingExceptions::_clearBeforeLoad()
{
  //s_apps.clear();
  //s_iconMap.clear();

  s_excluded.checkedApps.clear();
  s_excluded.uncheckedApps.clear();
  s_excluded.sortedApps.clear();

  s_included.checkedApps.clear();
  s_included.uncheckedApps.clear();
  s_included.sortedApps.clear();

  s_routes.clear();

  updateAllLists();

  for (auto *model : qAsConst (s_baseModels))
    model->refresh();
}

//void DapQmlModelRoutingExceptions::_delayedSave() const
//{
//  /* var */
//  QJsonArray japps, jroutes;

//  /* collect checked apps and their names */
//  for (const auto &app : qAsConst (s_checkedApps))
//    japps << toJson (app);

//  /* collect routes */
//  for (const auto &route : qAsConst (s_routes))
//    jroutes << toJson (route);

//  /* store */
//  QJsonObject jobj =
//  {
//    {"apps", japps},
//    {"routes", jroutes},
//  };

//  DapDataLocal::instance()->saveSetting (ROUTING_EXCEPTIONS_LIST, jobj);

//  /* clear data */
//  while (japps.erase (japps.begin())     != japps.end());
//  while (jroutes.erase (jroutes.begin()) != jroutes.end());
//}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelRoutingExceptions::rowCount (const QModelIndex &parent) const
{
  if (_model)
    return _model->rowCount (parent);
  return 0;
}

QVariant DapQmlModelRoutingExceptions::data (const QModelIndex &index, int role) const
{
  if (_model)
    return _model->data (index, role);
  return QVariant();
}

QHash<int, QByteArray> DapQmlModelRoutingExceptions::roleNames() const
{
  if (_model)
    return _model->roleNames();
  return QHash<int, QByteArray>();
}

/*-----------------------------------------*/

App toApp (const QJsonObject &a_src)
{
  if (a_src.contains ("checked_excluded"))
    return App
    {
      a_src.value ("packageName").toString(),
      a_src.value ("appName").toString(),
      {
        a_src.value ("checked_excluded").toBool(),
        a_src.value ("checked_included").toBool(),
      }
    };
  else
    return App
    {
      a_src.value ("packageName").toString(),
      a_src.value ("appName").toString(),
      {
        a_src.value ("checked").toBool(),
        false
      }
    };
}

App toApp (const QVariantMap &a_src)
{
  if (a_src.contains ("checked_excluded"))
    return App
    {
      a_src.value ("packageName").toString(),
      a_src.value ("appName").toString(),
      {
        a_src.value ("checked_excluded").toBool(),
        a_src.value ("checked_included").toBool(),
      }
    };
  else
    return App
    {
      a_src.value ("packageName").toString(),
      a_src.value ("appName").toString(),
      {
        a_src.value ("checked").toBool(),
        false
      }
    };
}

QString getIcon (const QJsonObject &a_src)
{
//  QImage icn;
//  QByteArray icnSrc (a_src.value ("icon").toString().toUtf8());
//  icn.loadFromData (QByteArray::fromBase64 (icnSrc));
//  return icn;
  return a_src.value ("icon").toString();
}

QString getIcon (const QVariantMap &a_src)
{
//  QImage icn;
//  QByteArray icnSrc (a_src.value ("icon").toString().toUtf8());
//  icn.loadFromData (QByteArray::fromBase64 (icnSrc));
//  return icn;
  return a_src.value ("icon").toString();
}

Route toRoute (const QJsonObject &a_src)
{
  return Route
  {
    a_src.value ("address").toString(),
    a_src.value ("description").toString(),
  };
}

Route toRoute (const QVariantMap &a_src)
{
  return Route
  {
    a_src.value ("address").toString(),
    a_src.value ("description").toString(),
  };
}

QJsonObject toJson (const App &a_src)
{
//  QBuffer buf;
//  buf.open (QIODevice::WriteOnly);
//  auto icnSrc  = getAppIcon (a_src.packageName);
//  icnSrc.save (&buf, "PNG");

//  QString icn = buf.readAll().toBase64();

  QString id  = (a_src.packageName.endsWith (".png")) ? a_src.packageName : a_src.packageName + ".png";

  return QJsonObject
  {
    { "packageName", a_src.packageName },
    { "appName", a_src.appName },
    { "icon", s_iconMap.value (id).data() },
    { "checked_excluded", a_src.checked.excluded },
    { "checked_included", a_src.checked.included },
  };
}

QJsonObject toJson (const Route &a_src)
{
  return QJsonObject
  {
    { "address",     a_src.address },
    { "description", a_src.description },
  };
}

#ifdef Q_OS_ANDROID
QImage getAppIcon (const QString &a_packageName)
{
  static QImage dummy;
  QString id  = (a_packageName.endsWith (".png")) ? a_packageName : a_packageName + ".png";
  if (!s_iconMap.contains (id))
    return dummy;
//  return s_iconMap [id];

  QImage icn;
  //QString iconBase64 = s_iconMap [id];
  icn.loadFromData (QByteArray::fromBase64 (s_iconMap [id]));
  return icn;
}
#endif // Q_OS_ANDROID

/*-----------------------------------------*/

RefreshingListModelBase::RefreshingListModelBase (QObject *parent)
  : QAbstractListModel (parent)
{

}

RefreshingListModelBase::~RefreshingListModelBase()
{

}

void RefreshingListModelBase::refresh()
{
  const char *name = "unknown";

  if (dynamic_cast<DqmreApps *> (this))
    name  = "DqmreApps";
  else if (dynamic_cast<DqmreRoutes *> (this))
    name  = "DqmreRoutes";
  else if (dynamic_cast<DqmreCheckedApps *> (this))
    name  = "DqmreCheckedApps";
  else if (dynamic_cast<DqmreSortedApps *> (this))
    name  = "DqmreSortedApps";

  qDebug() << "RefreshingListModelBase::refresh Mode:" << name;

  beginResetModel();
  endResetModel();
}

RefreshingListModel::RefreshingListModel (QObject *parent)
  : RefreshingListModelBase (parent)
{
  s_models.append (this);
}

RefreshingListModel::~RefreshingListModel()
{
  s_models.removeOne (this);
}

/*-----------------------------------------*/

DqmreApps::DqmreApps()
{

}

DqmreApps::~DqmreApps()
{

}

int DqmreApps::rowCount (const QModelIndex &) const
{
  return s_apps.size();
}

QVariant DqmreApps::data (const QModelIndex &index, int role) const
{
  if (index.row() < 0 || index.row() >= s_apps.size())
    return QVariant();

  Field field = Field (role);

  if (!s_allRoles.contains (field))
    return QVariant();

  switch (field)
    {
    case Field::packageName: return s_apps.at (index.row()).packageName;
    case Field::appName:     return s_apps.at (index.row()).appName;
    case Field::icon:
#ifdef Q_OS_ANDROID
      return getAppIcon (s_apps.at (index.row()).packageName); // s_apps.at (index.row()).icon;
#else // Q_OS_ANDROID
      return QVariant();
#endif // Q_OS_ANDROID

    default:
      return QVariant();
    }
}

QHash<int, QByteArray> DqmreApps::roleNames() const
{
  return s_appsRoles;
}

/*-----------------------------------------*/

DqmreRoutes::DqmreRoutes()
{
  s_baseModels.append (this);
}

DqmreRoutes::~DqmreRoutes()
{
  s_baseModels.removeOne (this);
}

int DqmreRoutes::rowCount (const QModelIndex &) const
{
  return s_routes.size();
}

QVariant DqmreRoutes::data (const QModelIndex &index, int role) const
{
  if (index.row() < 0 || index.row() >= s_routes.size())
    return QVariant();

  Field field = Field (role);

  if (!s_allRoles.contains (field))
    return QVariant();

  switch (field)
    {
    case Field::address:       return s_routes.at (index.row()).address;
    case Field::description:   return s_routes.at (index.row()).description;

    default:
      return QVariant();
    }
}

QHash<int, QByteArray> DqmreRoutes::roleNames() const
{
  return s_routesRoles;
}

/*-----------------------------------------*/

DqmreCheckedApps::DqmreCheckedApps (AppsContainer *a_container)
  : _container (a_container)
{
  if (_container == nullptr)
    qFatal ("%s : nullptr container provided", __func__);
  s_baseModels.append (this);
}

DqmreCheckedApps::~DqmreCheckedApps()
{
  s_baseModels.removeOne (this);
}

int DqmreCheckedApps::rowCount (const QModelIndex &) const
{
  return _container->checkedApps.size();
}

QVariant DqmreCheckedApps::data (const QModelIndex &index, int role) const
{
  if (index.row() < 0 || index.row() >= _container->checkedApps.size())
    return QVariant();

  Field field = Field (role);

  if (!s_allRoles.contains (field))
    return QVariant();

  switch (field)
    {
    case Field::packageName: return _container->checkedApps.at (index.row()).packageName;
    case Field::appName:     return _container->checkedApps.at (index.row()).appName;
    case Field::icon:
#ifdef Q_OS_ANDROID
      return getAppIcon (_container->checkedApps.at (index.row()).packageName); // s_checkedApps.at (index.row()).icon;
#else // Q_OS_ANDROID
      return QVariant();
#endif // Q_OS_ANDROID
    case Field::checked:
      return (_container->mode == Mode::EXC_CHECKED_APPS)
             ? _container->checkedApps.at (index.row()).checked.excluded
             : _container->checkedApps.at (index.row()).checked.included;

    default:
      return QVariant();
    }
}

QHash<int, QByteArray> DqmreCheckedApps::roleNames() const
{
  return s_appsRoles;
}

/*-----------------------------------------*/

DqmreSortedApps::DqmreSortedApps (AppsContainer *a_container)
  : _container (a_container)
{
  if (_container == nullptr)
    qFatal ("%s : nullptr container provided", __func__);
}

DqmreSortedApps::~DqmreSortedApps()
{

}

int DqmreSortedApps::rowCount (const QModelIndex &) const
{
  return _container->sortedApps.size();
}

QVariant DqmreSortedApps::data (const QModelIndex &index, int role) const
{
  if (index.row() < 0 || index.row() >= _container->sortedApps.size())
    return QVariant();

  Field field = Field (role);

  if (!s_allRoles.contains (field))
    return QVariant();

  switch (field)
    {
    case Field::packageName: return _container->sortedApps.at (index.row()).packageName;
    case Field::appName:     return _container->sortedApps.at (index.row()).appName;
    case Field::icon:
#ifdef Q_OS_ANDROID
      return getAppIcon (_container->sortedApps.at (index.row()).packageName); // s_sortedApps.at (index.row()).icon;
#else // Q_OS_ANDROID
      return QVariant();
#endif // Q_OS_ANDROID
    case Field::checked:
      return (_container->mode == Mode::EXC_CHECKED_APPS)
             ? _container->sortedApps.at (index.row()).checked.excluded
             : _container->sortedApps.at (index.row()).checked.included;

    default:
      return QVariant();
    }
}

QHash<int, QByteArray> DqmreSortedApps::roleNames() const
{
  return s_appsRoles;
}

/*-----------------------------------------*/

#ifdef Q_OS_ANDROID

DapQmlModelRoutingExceptionsImageProvider::DapQmlModelRoutingExceptionsImageProvider()
  : QQuickImageProvider (QQuickImageProvider::Pixmap)
{

}

DapQmlModelRoutingExceptionsImageProvider *DapQmlModelRoutingExceptionsImageProvider::instance()
{
  static DapQmlModelRoutingExceptionsImageProvider i;
  return &i;
}

QImage DapQmlModelRoutingExceptionsImageProvider::requestImage (const QString &id, QSize *size, const QSize &requestedSize)
{
  QImage result = getAppIcon (id);

  //auto type = result.format();

  if (size)
    *size = QSize (result.width(), result.height());

  if (requestedSize.width() > 0
      && requestedSize.height() > 0)
    result.scaled (requestedSize.width(), requestedSize.height());

  return result;
}

QPixmap DapQmlModelRoutingExceptionsImageProvider::requestPixmap (const QString &id, QSize *size, const QSize &requestedSize)
{
  return QPixmap::fromImage (requestImage (id, size, requestedSize));
}

#endif // Q_OS_ANDROID

/*-----------------------------------------*/

#ifdef Q_OS_ANDROID
AppIcon::operator QImage() const
{
  return getAppIcon (*this);
}
#endif // Q_OS_ANDROID

/*-----------------------------------------*/

DapQmlModelRoutingExceptionsFilterProxy::DapQmlModelRoutingExceptionsFilterProxy (QObject *parent)
  : QSortFilterProxyModel (parent)
{

}

void DapQmlModelRoutingExceptionsFilterProxy::setModel (QAbstractListModel *a_model)
{
  _model  = a_model;
  setSourceModel (a_model);
}

const QString &DapQmlModelRoutingExceptionsFilterProxy::filter() const
{
  return m_filter;
}

void DapQmlModelRoutingExceptionsFilterProxy::setFilter (const QString &a_filter)
{
  if (m_filter == a_filter)
    return;

  m_filter = a_filter;
  invalidateFilter();
}

bool DapQmlModelRoutingExceptionsFilterProxy::filterAcceptsRow (int sourceRow, const QModelIndex &sourceParent) const
{
  auto index  = _model->index (sourceRow, 0, sourceParent);
  auto name   = _model->data (index, int (Field::appName)).toString();
  return name.startsWith (m_filter, Qt::CaseInsensitive);
}

/*-----------------------------------------*/
