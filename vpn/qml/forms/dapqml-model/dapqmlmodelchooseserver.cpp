/* INCLUDES */
#include "dapqmlmodelchooseserver.h"
#include "DapServersData.h"

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

void DapQmlModelChooseServer::setServerManager(QSharedPointer<AbstractServerManager> a_serverManager)
{
  m_serverManager = a_serverManager;
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
  if (role != 0)
    return QVariant();

  if (m_serverManager.isNull())
    return DapServersData::instance()->data (index, Qt::DisplayRole);

  auto keys   = m_serverManager->keys();
  if (index.row() >= keys.size())
    return QVariant();

  return m_serverManager->server (keys.at (index.row())).name;
}

QHash<int, QByteArray> DapQmlModelChooseServer::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "name");

  return names;
}

/*-----------------------------------------*/
