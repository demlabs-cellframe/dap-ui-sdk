/* INCLUDES */
#include "dapqmlmodelchooseserver.h"
#include "DapServerList.h"

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
      auto *serverList          = DapServerList::instance();
      const auto &currentServer = serverList->currentServer();
      const auto name           = currentServer.name();
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
    return DapServerList::instance()->rowCount (parent);
  return m_serverManager->size();
}

QVariant DapQmlModelChooseServer::data(const QModelIndex &index, int role) const
{
  auto *serverList = DapServerList::instance();

  switch (role)
    {
    case Role::name:
      if (m_serverManager.isNull())
        return serverList->data (index, DapServerType::FieldId::name);

      if (index.row() >= m_serverManager->size())
        return QVariant();

      return m_serverManager->server (index.row()).name;

    case Role::ping:
        return serverList->data (index, DapServerType::FieldId::ping);

    case Role::connectionQuality:
      return serverList->data (index, DapServerType::FieldId::connQuality).toInt();

    case Role::checked:
        const auto &currentServer = serverList->currentServer();
        const auto name           = serverList->data (index, Qt::DisplayRole).toString();
        const auto address        = serverList->data (index, DapServerType::FieldId::address).toString();
        return (currentServer.name() == name
                && currentServer.address() == address);
    }

  return QVariant();
}

void DapQmlModelChooseServer::setCurrentServerByName (const QString &a_name)
{
  /* variables */
  auto *serverList = DapServerList::instance();

  /* lambdas */
  auto indexOf = [serverList] (const QString &a_serverName) -> int {

      int index = 0;
      for (const auto &server : qAsConst (*serverList))
        {
          if (server.name() == a_serverName)
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
  serverList->setCurrent (currentIndex);

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
