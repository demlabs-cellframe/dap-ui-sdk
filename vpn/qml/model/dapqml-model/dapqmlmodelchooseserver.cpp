/* INCLUDES */
#include "dapqmlmodelchooseserver.h"
#include "dapqmlmodelfullserverlist.h"

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

//#ifdef BRAND_KELVPN
//static int s_allowModelResetAmount = 2;
//#endif // BRAND_KELVPN

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelChooseServer::DapQmlModelChooseServer (QObject *parent)
  : QAbstractListModel (parent)
  , m_currentServer ("")
  , m_previousServer ("")
{
  /* vars */
  __inst  = this;

  connect (DapQmlModelFullServerList::instance(), &DapQmlModelFullServerList::currentChanged,
           this, &DapQmlModelChooseServer::currentChanged);

  // Universal model reset handling for all brands
  connect (DapQmlModelFullServerList::instance(), &QAbstractListModel::modelAboutToBeReset,
           this, [this]
  {
    beginResetModel();
  });
  connect (DapQmlModelFullServerList::instance(), &QAbstractListModel::modelReset,
           this, [this]
  {
    endResetModel();
  });

  QTimer::singleShot (500, [this]
  {
    auto *serverList          = DapQmlModelFullServerList::instance();
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

//void DapQmlModelChooseServer::setServerManager(QSharedPointer<AbstractServerManager> a_serverManager)
//{
//  //m_serverManager = a_serverManager;
//}

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
  // Force model reset to trigger UI update
  beginResetModel();
  endResetModel();
  
  // Emit refresh signal for QML components
  emit sigRefresh();
  
  // Force update of current server data
  auto *serverList = DapQmlModelFullServerList::instance();
  if (serverList) {
    // Trigger data refresh from underlying model
    serverList->refresh();
  }
}

int DapQmlModelChooseServer::current() const
{
  return DapQmlModelFullServerList::instance()->current();
}

void DapQmlModelChooseServer::setCurrent (int a_newCurrentServer)
{
  DapQmlModelFullServerList::instance()->setCurrent (a_newCurrentServer);
}

void DapQmlModelChooseServer::setCurrentServerByName (const QString &a_name)
{
  /* variables */
  auto *serverList = DapQmlModelFullServerList::instance();

  /* lambdas */
  auto indexOf = [serverList] (const QString & a_serverName) -> int
  {
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

QString DapQmlModelChooseServer::previousServer()
{
  return m_previousServer;
}

void DapQmlModelChooseServer::allowModelReset (int a_amount)
{
  // Universal model reset allowance for all brands
  // This method is kept for backward compatibility
  // The actual reset logic is now handled in the constructor
  Q_UNUSED(a_amount)
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelChooseServer::rowCount (const QModelIndex &parent) const
{
  //if (m_serverManager.isNull())
  return DapQmlModelFullServerList::instance()->rowCount (parent);
  //return m_serverManager->size();
}

QVariant DapQmlModelChooseServer::data (const QModelIndex &index, int role) const
{
  auto *serverList = DapQmlModelFullServerList::instance();

  switch (role)
    {
    case Role::name:
      //if (m_serverManager.isNull())
      return serverList->data (index, DapServerType::FieldId::name);

//      if (index.row() >= m_serverManager->size())
//        return QVariant();

//      return m_serverManager->server (index.row()).name();

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
