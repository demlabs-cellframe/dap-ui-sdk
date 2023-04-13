/* INCLUDES */
#include "dapqmlmodelroutingexceptions.h"

#include <QJsonObject>
#include <QJsonDocument>

/* DEFS */

#define CHECK_MODE(m) \
  if (mode() != m)\
    return;

#define CHECK_MODE_RET(m,v) \
  if (mode() != m)\
    return v;

typedef DapQmlModelRoutingExceptions::App App;
typedef DapQmlModelRoutingExceptions::Route Route;

// apps model

class DqmreApps : public QAbstractListModel
{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

// routes model

class DqmreRoutes : public QAbstractListModel
{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
};

enum Field
{
  invalid,
  name  = Qt::UserRole + 20,
  icon,
  address,
  description,
};

/* VARS */

static QList<App> s_apps;
static QList<Route> s_routes;

static const QHash<int, QByteArray> s_appsRoles =
  {
    {Field::name, "name"},
    {Field::icon, "icon"},
  };

static const QHash<int, QByteArray> s_routesRoles =
  {
    {Field::address,      "address"},
    {Field::description,  "description"},
  };

static const QHash<Field, QByteArray> s_allRoles =
  {
    {Field::name,         "name"},
    {Field::icon,         "icon"},
    {Field::address,      "address"},
    {Field::description,  "description"},
  };

/* FUNCS */

static App toApp (const QJsonObject &a_src);
static Route toRoute (const QJsonObject &a_src);
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
      || a_newMode < NONE
      || a_newMode > ROUTES)
    return;

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
    case APPS:    _model  = new DqmreApps;   break;
    case ROUTES:  _model  = new DqmreRoutes; break;
    }

  emit sigModeChanged();
}

void DapQmlModelRoutingExceptions::append (const DapQmlModelRoutingExceptions::App &a_app)
{
  CHECK_MODE (APPS);

  beginInsertRows (QModelIndex(), s_apps.size(), s_apps.size());
  s_apps.append (a_app);
  endInsertRows();
}

void DapQmlModelRoutingExceptions::append (DapQmlModelRoutingExceptions::App &&a_app)
{
  CHECK_MODE (APPS);

  beginInsertRows (QModelIndex(), s_apps.size(), s_apps.size());
  s_apps.append (std::move (a_app));
  endInsertRows();
}

void DapQmlModelRoutingExceptions::append (const DapQmlModelRoutingExceptions::Route &a_route)
{
  CHECK_MODE (ROUTES);

  beginInsertRows (QModelIndex(), s_routes.size(), s_routes.size());
  s_routes.append (a_route);
  endInsertRows();
}

void DapQmlModelRoutingExceptions::append (DapQmlModelRoutingExceptions::Route &&a_route)
{
  CHECK_MODE (ROUTES);

  beginInsertRows (QModelIndex(), s_routes.size(), s_routes.size());
  s_routes.append (std::move (a_route));
  endInsertRows();
}

void DapQmlModelRoutingExceptions::appendJson (const QVariant &a_value)
{
  /* get json */
  QJsonObject jobj  = a_value.toJsonObject();

  /* decide which mode this json refer to */
  Mode _mode  = NONE;
  if (jobj.contains ("name"))
    _mode = APPS;
  else if (jobj.contains ("address"))
    _mode = ROUTES;

  /* act to mode */
  switch (_mode)
    {
    case NONE: return;
    case APPS:    append (toApp (jobj));   break;
    case ROUTES:  append (toRoute (jobj)); break;
    }
}

void DapQmlModelRoutingExceptions::insert (int a_index, const DapQmlModelRoutingExceptions::App &a_app)
{
  CHECK_MODE (APPS);

  beginInsertRows (QModelIndex(), a_index, a_index);
  s_apps.insert (a_index, a_app);
  endInsertRows();
}

void DapQmlModelRoutingExceptions::insert (int a_index, DapQmlModelRoutingExceptions::App &&a_app)
{
  CHECK_MODE (APPS);

  beginInsertRows (QModelIndex(), a_index, a_index);
  s_apps.insert (a_index, std::move (a_app));
  endInsertRows();
}

void DapQmlModelRoutingExceptions::insert (int a_index, const DapQmlModelRoutingExceptions::Route &a_route)
{
  CHECK_MODE (ROUTES);

  beginInsertRows (QModelIndex(), a_index, a_index);
  s_routes.insert (a_index, a_route);
  endInsertRows();
}

void DapQmlModelRoutingExceptions::insert (int a_index, DapQmlModelRoutingExceptions::Route &&a_route)
{
  CHECK_MODE (ROUTES);

  beginInsertRows (QModelIndex(), a_index, a_index);
  s_routes.insert (a_index, std::move (a_route));
  endInsertRows();
}

void DapQmlModelRoutingExceptions::appendJson (int a_index, const QVariant &a_value)
{
  /* get json */
  QJsonObject jobj  = a_value.toJsonObject();

  /* decide which mode this json refer to */
  Mode _mode  = NONE;
  if (jobj.contains ("name"))
    _mode = APPS;
  else if (jobj.contains ("address"))
    _mode = ROUTES;

  /* act to mode */
  switch (_mode)
    {
    case NONE: return;
    case APPS:    insert (a_index, toApp (jobj));   break;
    case ROUTES:  insert (a_index, toRoute (jobj)); break;
    }
}

const DapQmlModelRoutingExceptions::App &DapQmlModelRoutingExceptions::app (int a_index) const
{
  static const App dummy;
  CHECK_MODE_RET (APPS, dummy);

  return s_apps.at (a_index);
}

const DapQmlModelRoutingExceptions::Route &DapQmlModelRoutingExceptions::route (int a_index) const
{
  static Route dummy;
  CHECK_MODE_RET (ROUTES, dummy);

  return s_routes.at (a_index);
}

QVariant DapQmlModelRoutingExceptions::appJson (int a_index) const
{
  static const QVariant dummy;
  CHECK_MODE_RET (APPS, dummy);

  return toJson (s_apps.at (a_index));
}

QVariant DapQmlModelRoutingExceptions::routeJson (int a_index) const
{
  static const QVariant dummy;
  CHECK_MODE_RET (ROUTES, dummy);

  return toJson (s_routes.at (a_index));
}

void DapQmlModelRoutingExceptions::removeApp (int a_index)
{
  CHECK_MODE (APPS);

  beginRemoveRows (QModelIndex(), a_index, a_index);
  s_apps.removeAt (a_index);
  endRemoveRows();
}

void DapQmlModelRoutingExceptions::removeRoute (int a_index)
{
  CHECK_MODE (ROUTES);

  beginRemoveRows (QModelIndex(), a_index, a_index);
  s_routes.removeAt (a_index);
  endRemoveRows();
}

void DapQmlModelRoutingExceptions::moveApp (int a_from, int a_to)
{
  CHECK_MODE (APPS);

  QModelIndex dummy;
  beginMoveRows (dummy, a_from, a_from, dummy, a_to);
  s_apps.move (a_from, a_to);
  endMoveRows();
}

void DapQmlModelRoutingExceptions::moveRoute (int a_from, int a_to)
{
  CHECK_MODE (ROUTES);

  QModelIndex dummy;
  beginMoveRows (dummy, a_from, a_from, dummy, a_to);
  s_routes.move (a_from, a_to);
  endMoveRows();
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
  return App
  {
    a_src.value ("name").toString(),
    a_src.value ("icon").toString(),
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

QJsonObject toJson (const App &a_src)
{
  return QJsonObject
  {
    { "name", a_src.name },
    { "icon", a_src.icon },
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
    case name:  return s_apps.at (index.row()).name;
    case icon:  return s_apps.at (index.row()).icon;

    case invalid:
    default:
      return QVariant();
    }
}

QHash<int, QByteArray> DqmreApps::roleNames() const
{
  return s_appsRoles;
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
