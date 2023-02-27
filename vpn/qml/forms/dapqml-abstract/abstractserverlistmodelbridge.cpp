/* INCLUDES */
#include "abstractserverlistmodelbridge.h"

/* DEFS */

class DeafultserverListModelBridge : public AbstractServerListModelBridge
{
  DapServerList *m_serverList;
  DapQmlModelAutoServerList *m_autoServerList;
public:
  DeafultserverListModelBridge()
    : m_serverList (DapServerList::instance())
    , m_autoServerList (new DapQmlModelAutoServerList)
  {

  }
  DapServerList &serverList()                 { return *m_serverList; }
  DapQmlModelAutoServerList &autoServerList() { return *m_autoServerList; }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

AbstractServerListModelBridge::AbstractServerListModelBridge()
{

}

AbstractServerListModelBridge::~AbstractServerListModelBridge()
{

}

/********************************************
 * METHODS
 *******************************************/

AbstractServerListModelBridge *AbstractServerListModelBridge::getDefaultBridge()
{
  static DeafultserverListModelBridge i;
  return &i;
}

/********************************************
 * OVERRIDE
 *******************************************/

QVariant AbstractServerListModelBridge::customData (const QModelIndex &, int , int , int ) const
{
  return QVariant();
}

const QHash<int, QByteArray> &AbstractServerListModelBridge::customRoleNames() const
{
  static QHash<int, QByteArray> i;
  return i;
}

/*-----------------------------------------*/
