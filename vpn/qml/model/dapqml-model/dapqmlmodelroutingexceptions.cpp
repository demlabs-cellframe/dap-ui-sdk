/* INCLUDES */
#include "dapqmlmodelroutingexceptions.h"

#include <QLinkedList>
#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "DapDataLocal.h"

/* DEFS */

#define TEST_MODE

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

#ifdef TEST_MODE
  if (s_apps.isEmpty())
    {
      append (App{"TestApp1", "qrc:/nonthemed/conn-icon-3.png", false, QImage()});
      append (App{"TestApp2", "qrc:/nonthemed/conn-icon-4.png", true,  QImage()});
      append (App{"TestApp3", "qrc:/nonthemed/conn-icon-5.png", false, QImage()});
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

void DapQmlModelRoutingExceptions::updateAllLists()
{
  for (auto *model : qAsConst (s_models))
    model->refresh();
}

void DapQmlModelRoutingExceptions::save() const
{
  QJsonArray /*japps, */jroutes;

  //for (const auto &app : qAsConst(s_apps))
  //  japps << toJson (app);

  for (const auto &route : qAsConst(s_routes))
    jroutes << toJson (route);

  QJsonObject jobj =
  {
    //{"apps", japps},
    {"routes", jroutes},
  };

  DapDataLocal::instance()->saveSetting (SETTING_ROUTING_EXCEPTIONS, jobj);
}

void DapQmlModelRoutingExceptions::load()
{
  auto jobj     = DapDataLocal::instance()->getSetting (SETTING_ROUTING_EXCEPTIONS).toJsonObject();
  auto //japps    = jobj.value ("apps").toArray(),
       jroutes  = jobj.value ("routes").toArray();

  clearRoutes(); // clear();

//  for (const auto &app : qAsConst (japps))
//    append (toApp (app.toObject()));

  for (const auto &route : qAsConst (jroutes))
    append (toRoute (route.toObject()));
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
  if (checked)
    removeFrom (s_uncheckedApps, app);
  else
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
    case packageName: return s_apps.at (index.row()).packageName;
    case appName:     return s_apps.at (index.row()).appName;
    case icon:        return s_apps.at (index.row()).icon;
    case checked:     return s_apps.at (index.row()).checked;

    case invalid:
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
    case address:       return s_routes.at (index.row()).address;
    case description:   return s_routes.at (index.row()).description;

    case invalid:
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
    case packageName: return s_checkedApps.at (index.row()).packageName;
    case appName:     return s_checkedApps.at (index.row()).appName;
    case icon:        return s_checkedApps.at (index.row()).icon;
    case checked:     return s_checkedApps.at (index.row()).checked;

    case invalid:
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
    case packageName: return s_sortedApps.at (index.row()).packageName;
    case appName:     return s_sortedApps.at (index.row()).appName;
    case icon:        return s_sortedApps.at (index.row()).icon;
    case checked:     return s_sortedApps.at (index.row()).checked;

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
