/* INCLUDES */
#include "dapqmlmodelchooseserver.h"
#include "DapServersData.h"

/* DEFS */
enum Role
{
  ping,
  name,
  connectionQuality,
  checked
};

/* VARS */
static DapQmlModelChooseServer *__inst = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelChooseServer::DapQmlModelChooseServer (QObject *parent)
  : QAbstractListModel (parent)
  , m_currentServer("")
  , m_previousServer("")
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

void DapQmlModelChooseServer::setServerManager(QSharedPointer<AbstractServerManager> a_serverManager)
{
  //m_serverManager = a_serverManager;
}

QString DapQmlModelChooseServer::hook()
{
  return QString();
}

int DapQmlModelChooseServer::hookInt()
{
  return 0;
}

void DapQmlModelChooseServer::refresh()
{
//  beginResetModel();
//  endResetModel();
  emit sigRefresh();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelChooseServer::rowCount(const QModelIndex &parent) const
{
  if (m_serverManager.isNull())
    return DapServersData::instance()->rowCount (parent);
  return m_serverManager->size();
}

QVariant DapQmlModelChooseServer::data(const QModelIndex &index, int role) const
{
  auto *serversData = DapServersData::instance();

  switch (role)
    {
    case Role::name:
      if (m_serverManager.isNull())
        return DapServersData::instance()->data (index, Qt::DisplayRole);

      if (index.row() >= m_serverManager->size())
        return QVariant();

      return m_serverManager->server (index.row()).name;

    case Role::ping:
        return serversData->data (index, PING_ROLE);

    case Role::connectionQuality:
      return serversData->data (index, CONNECTION_QUALITY).toInt();

    case Role::checked:
        const auto &currentServer   = serversData->currentServer();
        const auto name             = serversData->data (index, Qt::DisplayRole).toString();
        const auto address          = serversData->data (index, ADDRESS_ROLE).toString();
        return (currentServer.name == name
                && currentServer.address == address);
    }

  return QVariant();
}

/// set checked item
void DapQmlModelChooseServer::setCheckedServer(QString name)
{
  int oldIndex = -1;
  QModelIndex i = DapServersData::instance()->indexOf(m_currentServer);
  if (i.isValid())
    oldIndex = i.row();
  m_previousServer = m_currentServer;
  m_currentServer = name;
  i = DapServersData::instance()->indexOf(m_currentServer);
  if (i.isValid())
  {
    // set unchecked item
    if (oldIndex >= 0)
      emit dataChanged(index(oldIndex, 0), index(oldIndex, 0));
    // set checked item
    emit dataChanged(index(i.row(), 0), index(i.row(), 0));
  }
}

QHash<int, QByteArray> DapQmlModelChooseServer::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (Role::name,               "name");
  names.insert (Role::ping,               "ping");
  names.insert (Role::connectionQuality,  "connectionQuality");
  names.insert (Role::checked,            "checked");

  return names;
}
/*-----------------------------------------*/
