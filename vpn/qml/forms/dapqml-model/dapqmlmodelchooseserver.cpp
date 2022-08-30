/* INCLUDES */
#include "dapqmlmodelchooseserver.h"
#include "DapServersData.h"

/* DEFS */
enum Role
{
  ping,
  name,
  connectionQuality
};

/* VARS */
static DapQmlModelChooseServer *__inst = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelChooseServer::DapQmlModelChooseServer (QObject *parent)
  : QAbstractListModel (parent)
{
  /* vars */
  __inst  = this;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelChooseServer *DapQmlModelChooseServer::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelChooseServer;
  return __inst;
}

void DapQmlModelChooseServer::refresh()
{
  beginResetModel();
  endResetModel();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelChooseServer::rowCount(const QModelIndex &parent) const
{
  return DapServersData::instance()->rowCount (parent);
}

QVariant DapQmlModelChooseServer::data(const QModelIndex &index, int role) const
{
  auto *serversData = DapServersData::instance();

  switch (role)
    {
    case Role::name:
      return serversData->data (index, Qt::DisplayRole);

    case Role::ping:
        return serversData->data (index, PING_ROLE);

    case Role::connectionQuality:
      return serversData->data (index, CONNECTION_QUALITY).toInt();
    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelChooseServer::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (Role::name,               "name");
  names.insert (Role::ping,               "ping");
  names.insert (Role::connectionQuality,  "connectionQuality");

  return names;
}

//QVariant DapQmlModelChooseServer::value(int a_row, const QString &a_name)
//{
//  auto roles  = roleNames();
//  int role    = roles.key (a_name.toUtf8());
//  auto index  = this->index (a_row);
//  return data (index, role);
//}

/*-----------------------------------------*/
