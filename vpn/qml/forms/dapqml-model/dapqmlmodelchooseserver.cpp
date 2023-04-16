/* INCLUDES */
#include "dapqmlmodelchooseserver.h"
#include "DapServersData.h"

#include <QTimer>

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

  QTimer::singleShot (500, [this] {
      auto *serversData         = DapServersData::instance();
      const auto &currentServer = serversData->currentServer();
      const auto name           = currentServer.name;
      if (m_currentServer.isEmpty())
        m_currentServer         = name;
    });
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
        const auto &currentServer = serversData->currentServer();
        const auto name           = serversData->data (index, Qt::DisplayRole).toString();
        const auto address        = serversData->data (index, ADDRESS_ROLE).toString();
        return (currentServer.name == name
                && currentServer.address == address);
    }

  return QVariant();
}

void DapQmlModelChooseServer::setCurrentServerByName (const QString &a_name)
{
  /* variables */
  auto *serversData   = DapServersData::instance();
  const auto &servers = serversData->servers();

  /* lambdas */
  auto indexOf = [servers] (const QString &a_serverName) -> int {

      int index = 0;
      for (const auto &server : servers)
        {
          if (server.name == a_serverName)
            return index;
          index++;
        }

      return -1;
    };

  /* store changes */
  m_previousServer  = m_currentServer;
  m_currentServer   = a_name;

  /* get indexes */
  int currentIndex  = indexOf (m_currentServer);
  int lastIndex     = indexOf (m_previousServer);

//  qDebug() << __PRETTY_FUNCTION__
//           << "current:" << currentIndex
//           << ",last:" << lastIndex
//           << ",names:" << m_currentServer << m_previousServer;

  /* update current server */
  serversData->setCurrentServer (currentIndex);

  /* send update notifies */
  if (lastIndex != -1 && lastIndex != currentIndex)
    emit dataChanged (index (lastIndex, 0),     index (lastIndex, 0));
  if (currentIndex != -1)
    emit dataChanged (index (currentIndex, 0),  index (currentIndex, 0));
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
