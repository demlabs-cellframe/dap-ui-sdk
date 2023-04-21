/* INCLUDES */
#include "dapqmlmodelroutingexceptions.h"

#include <QLinkedList>
#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "DapDataLocal.h"

/* DEFS */

//#define TEST_MODE

typedef DapQmlModelRoutingExceptions::App App;
typedef DapQmlModelRoutingExceptions::Route Route;
typedef DapQmlModelRoutingExceptions::Field Field;

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

class DqmreRoutes : public RefreshingListModelBase
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
public:
  DqmreCheckedApps();
  ~DqmreCheckedApps();
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

// checked apps model

class DqmreSortedApps : public RefreshingListModel
{
public:
  DqmreSortedApps();
  ~DqmreSortedApps();
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

/* VARS */

static const QString SETTING_ROUTING_EXCEPTIONS {"RoutingExceptions"};

// all apps list
static QList<App> s_apps;
// all routes list
static QList<Route> s_routes;
// apps icons map
static QMap<QString, QImage> s_iconMap;
// only checked apps
static QList<App> s_checkedApps;
// only unchecked apps
static QList<App> s_uncheckedApps;
// sorted apps : checked at top, unchecked at the bottom
static QList<App> s_sortedApps;

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
    "iVBORw0KGgoAAAANSUhEUgAAAGAAAABgCAYAAADimHc4AAAACXBIWXMAACE4AAAhOAFFljFgAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAV0SURBVHgB7Z2/bxtlGMe/54SmlYrqKqgViII7ABIiaqwGFpAa1A0GwsgU5y9IMqA0WepOSVjqbmyxJzYoS7cqiQSqBK7iqIhKVAiHIlCCkC4CQYJaH+/3zLVuYjt27u59avv5SG6c87lWnu/z6318p9dBK2TXkhh4MAzPGYODC+aRMkeTUGpxzaOECtbNzxXMjVxv5U1O01dp+KOVScCbghq8TbwykMjDSxQwmy43OquxAJ8UJ42aWajhQ2KEcPqzmEkX6r1aX4DF21fh+V6vRIXj5DBzfnrf4X0nLhSXzL8ZKDHg5HHp/ETtkcQTr9Pz1fgx4mX+t/EjHkfAfDFjfluCEj+ON42ZN3P+U//A/K0UnCPLRqEUFBu42Ok7i2zaraYg58i4Gt8qXFf5TU41AhZu/6QCWMePggQWi2NqfBH86UICHkahyGBGO6wB56DI0IcLFGAYigweUhRAZz1yJBNQRFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhFEBhOlHj5Ic6MPwqWMYH3oOoy8dR+rEAMrb/2LiRhkrP/8JWzhYKHroIUbPHMfYqycx/sYgkkf79r1OEc5+ege26IkIoLePDw1i7JWk8fZnm56bOnEENulqAQ7y9qeBrhOgHW+vB1OQTbpGABbUjCmoYb29vL0Lm3S8AEwzl9954VDeXo/t3YewSUcKwDQzOXIKUyOnI8/tpc2/YZOOEsBGUV37/R/YpCMEiDrNNGN7R1OQT9g0w2I6ffMXXL/nYi3zOoZPH2vpfT2fgqJIM9eKW8h+9StcU1DZHbVqfJ7v9moRjiLN0Osnbmw8muXQ+Msfvdby+217PxEVIEgz7N/DjgBqvZ4Exm8nimy3oEREABqbKSaKNnKv15PDGJ90fQRE3c3s9XpyWOMT2y0oiV2AsLOZetTzesKoyl08c+io2nDtjiFIbALEtVqt5/WExs+/n0IYSltdEgFhPbEejbyeUGh+XhhKW/bzP4lFgKzJ8za8nlx++3n/88KyYXkMHRCLAPSmKL5Zaub1JCrjB58lQSxXRXz4+Y+hv9ym16eX7jb8P65efDEy45Pl+39BgtiKcP7OH/6DkcDu5wPTBbETOoiDvJ4svfeyv3iLEttDuADrV0VQDApxzvTrtW0pDb5qvDD37WbTeUwcxicncyXrcyBifSVMQ9d6N6/HcXcetPTHx2V81iwJ4xPxYVyrxY85Pw7jE6kOiHTEpYlc1HFBFxcSM6AAvTYUMjOggI4QgPn5yte/IS4kZkABHRMBXAlP37xvCnb0xVJiBhTQUSkox8VZ/vtIr16WmgEFdFwN4KWD7372Q2TRINkBkY4twkE0hO1gJDsg0tFdEKMhnb8bqkCvCM2AArqiDWWB5k0Vh5loagRERPXOlu/aigaJ64D20nULsXaiYdXivWCN6MqVcKvR8MU9F9J09SiiWTTwWMF8XyFNz9wlmRka9L8U4mBv3ax8c8VN67cj1aPnblN92tBpqDAqgDAqgDAqgDAqgDAqgDAqgDAqgDAqgDAqgDAqgDAUQH4m27u43EesDEWKUsLMQ1ehyFDBuokAr6XN55UYeKZvOYHd/hK0DgjglfFx+ssE97Q1UXANil08FPij2obu9nN3Z40CaxjvR3+ez6oCMAoq3hUodnD6s5hNl/n08UJszt/jXFNR3FSMjWfSheBXZ98J89/k4STGoUSPVylg9q1M7aH9o4jqCRoJUUPP32N8Un8WdGlkylTpCRMgZShhceF405gzNq2D0/St87dSplpnqilJ951vk2p7zw6TTU4DHLTKYnEMDzFqYoZboHMXbt0I+klcf67mmdGOY6YLXOA2MXzAf1OiLlNAMR3NAAAAAElFTkSuQmCC",
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
static Route toRoute (const QJsonObject &a_src);
static Route toRoute (const QVariantMap &a_src);
static QJsonObject toJson (const App &a_src);
static QJsonObject toJson (const Route &a_src);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelRoutingExceptions::DapQmlModelRoutingExceptions()
  : m_mode (NONE)
  , _model (nullptr)
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
      || ! (a_newMode & _ALL))
    return;

  beginResetModel();

  /* store mode */
  m_mode  = Mode (a_newMode);

  /* delete model, if present */
  if (_model)
    delete _model;

  /* clear */
  _model  = nullptr;

  /* set new model based on new mode */
  switch (m_mode)
    {
    case NONE: return;
    case CHECKED_APPS:    _model  = new DqmreCheckedApps; break;
    case SORTED_APPS:     _model  = new DqmreSortedApps;  break;
    case ALL_APPS:        _model  = new DqmreApps;        break;
    case ALL_ROUTES:      _model  = new DqmreRoutes;      break;
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
          QImage icn;
          QByteArray icnSrc (dummyApp.icon.toUtf8());
          icn.loadFromData (QByteArray::fromBase64 (icnSrc));
          append (App {dummyApp.pname, dummyApp.aname, dummyApp.checked, icn});
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

void DapQmlModelRoutingExceptions::append (const DapQmlModelRoutingExceptions::App &a_app)
{
  beginInsertRows (QModelIndex(), s_apps.size(), s_apps.size());
  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
  s_apps.append (a_app);
  endInsertRows();

  _appendCheckedApp (s_apps.size() - 1);
}

void DapQmlModelRoutingExceptions::append (DapQmlModelRoutingExceptions::App &&a_app)
{
  beginInsertRows (QModelIndex(), s_apps.size(), s_apps.size());
  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
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
    case ALL_APPS:    append (toApp (vmap));   break;
    case ALL_ROUTES:  append (toRoute (vmap)); break;
    default: break;
    }
}

void DapQmlModelRoutingExceptions::insert (int a_index, const DapQmlModelRoutingExceptions::App &a_app)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
  s_apps.insert (a_index, a_app);
  endInsertRows();

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::insert (int a_index, DapQmlModelRoutingExceptions::App &&a_app)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
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
    case ALL_APPS:    insert (a_index, toApp (vmap));   break;
    case ALL_ROUTES:  insert (a_index, toRoute (vmap)); break;
    default: break;
    }
}

void DapQmlModelRoutingExceptions::replace (int a_index, const DapQmlModelRoutingExceptions::App &a_app)
{
  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
  s_apps[a_index] = a_app;
  auto changedIndex = index (a_index);
  emit dataChanged (changedIndex, changedIndex);

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::replace (int a_index, DapQmlModelRoutingExceptions::App &&a_app)
{
  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
  s_apps[a_index] = std::move (a_app);
  auto changedIndex = index (a_index);
  emit dataChanged (changedIndex, changedIndex);

  _sortCheckedApps();
}

void DapQmlModelRoutingExceptions::replaceSorted (int a_index, const DapQmlModelRoutingExceptions::App &a_app)
{
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

  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
  s_sortedApps[a_index] = a_app;
  auto changedIndex = this->index (a_index);
  emit dataChanged (changedIndex, changedIndex);
}

void DapQmlModelRoutingExceptions::replaceSorted (int a_index, DapQmlModelRoutingExceptions::App &&a_app)
{
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

  s_iconMap[a_app.packageName + ".png"] = a_app.icon;
  s_sortedApps[a_index] = std::move (a_app);
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
  return s_sortedApps.at (a_index);
}

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
  App &app  = s_checkedApps[a_index];
  int result = 0;
  for (auto i = s_apps.cbegin(), e = s_apps.cend(); i != e; i++, result++)
    if (i->packageName == app.packageName)
      return result;
  return -1;
}

int DapQmlModelRoutingExceptions::indexOfSorted (int a_index) const
{
  App &app  = s_sortedApps[a_index];
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

void DapQmlModelRoutingExceptions::removeAppFromChecked(int a_index)
{
  int index = indexOfChecked (a_index);

  beginRemoveRows (QModelIndex(), a_index, a_index);
  App &app      = s_apps[index];
  app.checked   = false;
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

//void DapQmlModelRoutingExceptions::save() const
//{
//  QJsonArray /*japps, */jroutes;

//  //for (const auto &app : qAsConst(s_apps))
//  //  japps << toJson (app);

//  for (const auto &route : qAsConst(s_routes))
//    jroutes << toJson (route);

//  QJsonObject jobj =
//  {
//    //{"apps", japps},
//    {"routes", jroutes},
//  };

//  DapDataLocal::instance()->saveSetting (SETTING_ROUTING_EXCEPTIONS, jobj);
//}

//void DapQmlModelRoutingExceptions::load()
//{
//  auto jobj     = DapDataLocal::instance()->getSetting (SETTING_ROUTING_EXCEPTIONS).toJsonObject();
//  auto //japps    = jobj.value ("apps").toArray(),
//       jroutes  = jobj.value ("routes").toArray();

//  clearRoutes(); // clear();

////  for (const auto &app : qAsConst (japps))
////    append (toApp (app.toObject()));

//  for (const auto &route : qAsConst (jroutes))
//    append (toRoute (route.toObject()));
//}

void DapQmlModelRoutingExceptions::clear()
{
  clearApps();
  clearRoutes();
}

void DapQmlModelRoutingExceptions::clearApps()
{
  s_apps.clear();
  s_iconMap.clear();
  s_checkedApps.clear();
  s_uncheckedApps.clear();
  s_sortedApps.clear();

  updateAllLists();

  for (auto *model : qAsConst (s_baseModels))
    model->refresh();
}

void DapQmlModelRoutingExceptions::clearRoutes()
{
  s_routes.clear();
  updateAllLists();
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
  auto &app     = s_apps.at (a_index);
  bool checked  = app.checked;

  /* remove from old list */
  //if (checked)
  removeFrom (s_uncheckedApps, app);
  //else
  removeFrom (s_checkedApps, app);

  /* append to desired list */
  if (checked)
    s_checkedApps.append (app);
  else
    s_uncheckedApps.append (app);

  /* combine new list */
  if (a_combine)
    {
      s_sortedApps  = s_checkedApps + s_uncheckedApps;
      updateAllLists();
    }
}

void DapQmlModelRoutingExceptions::_removeCheckedApp (int a_index)
{
  /* variables */
  auto &app     = s_apps.at (a_index);
  bool checked  = app.checked;

  /* remove from old list */
  if (checked)
    removeFrom (s_uncheckedApps, app);
  else
    removeFrom (s_checkedApps, app);
}

void DapQmlModelRoutingExceptions::_sortCheckedApps()
{
  /* reinsert all items */
  for (int i = 0; i < s_apps.size(); i++)
    _appendCheckedApp (i, false);

  /* combine new list */
  s_sortedApps  = s_checkedApps + s_uncheckedApps;
  updateAllLists();
}

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
  QImage icn;
  QByteArray icnSrc (a_src.value ("icon").toString().toUtf8());
  icn.loadFromData (QByteArray::fromBase64 (icnSrc));

  return App
  {
    a_src.value ("packageName").toString(),
    a_src.value ("appName").toString(),
    a_src.value ("checked").toBool(),
    icn
  };
}

App toApp (const QVariantMap &a_src)
{
  QImage icn;
  QByteArray icnSrc (a_src.value ("icon").toString().toUtf8());
  icn.loadFromData (QByteArray::fromBase64 (icnSrc));

  return App
  {
    a_src.value ("packageName").toString(),
    a_src.value ("appName").toString(),
    a_src.value ("checked").toBool(),
    icn
  };
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
  QBuffer buf;
  buf.open (QIODevice::WriteOnly);
  a_src.icon.save (&buf, "PNG");

  QString icn = buf.readAll().toBase64();

  return QJsonObject
  {
    { "packageName", a_src.packageName },
    { "appName", a_src.appName },
    { "icon", icn },
    { "checked", a_src.checked },
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

/*-----------------------------------------*/

RefreshingListModelBase::RefreshingListModelBase(QObject *parent)
  : QAbstractListModel (parent)
{

}

RefreshingListModelBase::~RefreshingListModelBase()
{

}

void RefreshingListModelBase::refresh()
{
  const char *name = "unknown";
  if (dynamic_cast<DqmreApps*>(this))
    name  = "DqmreApps";
  else
    if (dynamic_cast<DqmreRoutes*>(this))
      name  = "DqmreRoutes";
    else
      if (dynamic_cast<DqmreCheckedApps*>(this))
        name  = "DqmreCheckedApps";
      else
        if (dynamic_cast<DqmreSortedApps*>(this))
          name  = "DqmreSortedApps";
  qDebug() << __PRETTY_FUNCTION__ << "Mode:" << name;

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
  Field field = Field (role);

  if (!s_allRoles.contains (field))
    return QVariant();

  switch (field)
    {
    case Field::packageName: return s_apps.at (index.row()).packageName;
    case Field::appName:     return s_apps.at (index.row()).appName;
    case Field::icon:        return s_apps.at (index.row()).icon;
    case Field::checked:     return s_apps.at (index.row()).checked;

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

DqmreCheckedApps::DqmreCheckedApps()
{
  s_baseModels.append (this);
}

DqmreCheckedApps::~DqmreCheckedApps()
{
  s_baseModels.removeOne (this);
}

int DqmreCheckedApps::rowCount (const QModelIndex &) const
{
  return s_checkedApps.size();
}

QVariant DqmreCheckedApps::data (const QModelIndex &index, int role) const
{
  Field field = Field (role);

  if (!s_allRoles.contains (field))
    return QVariant();

  switch (field)
    {
    case Field::packageName: return s_checkedApps.at (index.row()).packageName;
    case Field::appName:     return s_checkedApps.at (index.row()).appName;
    case Field::icon:        return s_checkedApps.at (index.row()).icon;
    case Field::checked:     return s_checkedApps.at (index.row()).checked;

    default:
      return QVariant();
    }
}

QHash<int, QByteArray> DqmreCheckedApps::roleNames() const
{
  return s_appsRoles;
}

/*-----------------------------------------*/

DqmreSortedApps::DqmreSortedApps()
{

}

DqmreSortedApps::~DqmreSortedApps()
{

}

int DqmreSortedApps::rowCount (const QModelIndex &) const
{
  return s_sortedApps.size();
}

QVariant DqmreSortedApps::data (const QModelIndex &index, int role) const
{
  Field field = Field (role);

  if (!s_allRoles.contains (field))
    return QVariant();

  switch (field)
    {
    case Field::packageName: return s_sortedApps.at (index.row()).packageName;
    case Field::appName:     return s_sortedApps.at (index.row()).appName;
    case Field::icon:        return s_sortedApps.at (index.row()).icon;
    case Field::checked:     return s_sortedApps.at (index.row()).checked;

    default:
      return QVariant();
    }
}

QHash<int, QByteArray> DqmreSortedApps::roleNames() const
{
  return s_appsRoles;
}

/*-----------------------------------------*/

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
  QImage result = s_iconMap.value (id);

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

/*-----------------------------------------*/
