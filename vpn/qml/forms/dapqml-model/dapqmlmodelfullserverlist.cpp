/* INCLUDES */
#include "dapqmlmodelfullserverlist.h"
#include "dapqml-abstract/abstractserverlistmodelbridge.h"
#include "DapServerInfo.h"

#include <QMetaEnum>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelFullServerList::DapQmlModelFullServerList()
  : m_bridge (AbstractServerListModelBridge::getDefaultBridge())
{
  _getSizes();
  _getRoles();
}

/********************************************
 * METHODS
 *******************************************/

const AbstractServerListModelBridge *DapQmlModelFullServerList::bridge() const
{
  return m_bridge;
}

void DapQmlModelFullServerList::setBridge(AbstractServerListModelBridge *a_newBridge)
{
  beginResetModel();
  m_bridge  = a_newBridge;
  _getSizes();
  _getRoles();
  endResetModel();
}

QVariant DapQmlModelFullServerList::value (int a_row, const QString &a_name)
{
  int fieldId = _roleNamesMap.key (a_name.toUtf8(), -1);

  if (fieldId == -1)
    return QVariant();

  return data (index (a_row, 0), fieldId);
}

void DapQmlModelFullServerList::_getSizes()
{
  auto &autoServerList  = m_bridge->autoServerList();
  auto &serverList      = m_bridge->serverList();
  _size.autoServer      = autoServerList.rowCount (QModelIndex());
  _size.server          = serverList.rowCount (QModelIndex());
  _size.full            = _size.autoServer + _size.server;
}

void DapQmlModelFullServerList::_getRoles()
{
  _roleNamesMap       = _baseRoleNames();
  const auto &custom  =  m_bridge->customRoleNames();

  for (auto i = custom.begin(), e = custom.end(); i != e; i++)
    _roleNamesMap.insert (i.key(), i.value());
}

const QHash<int, QByteArray> &DapQmlModelFullServerList::_baseRoleNames()
{
  static QHash<int, QByteArray> result;

  if (result.isEmpty())
    {
      int enumIndex  = DapServerType::staticMetaObject.indexOfEnumerator ("FieldId");
      auto metaEnum  = DapServerType::staticMetaObject.enumerator (enumIndex);
      int enumSize   = metaEnum.keyCount();

      for (int i = 0; i < enumSize; i++)
        result.insert (metaEnum.value (i), metaEnum.key (i));
    }

  return result;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelFullServerList::rowCount (const QModelIndex &parent) const
{
  return m_bridge->serverList()     .rowCount (parent)
       + m_bridge->autoServerList() .rowCount (parent);
}

QVariant DapQmlModelFullServerList::data (const QModelIndex &index, int role) const
{
  auto &autoServerList  = m_bridge->autoServerList();
  auto &serverList      = m_bridge->serverList();
  auto &customRoleNames = m_bridge->customRoleNames();

  /* check boundaries */
  if (index.row() < 0 || index.row() >= _size.full)
    return QVariant();

  /* handle custom */
  if (customRoleNames.contains (role))
    return m_bridge->customData (index, role, _size.autoServer, _size.server);

  /* if auto server boundaries */
  if (index.row() < _size.autoServer)
    return autoServerList.data (index, role);

  /* server boundaries */
  int newIndexValue     = index.row() - _size.autoServer;
  QModelIndex newIndex  = QAbstractListModel::index (newIndexValue, index.column(), index.parent());
  return serverList.data (newIndex, role);
}

QHash<int, QByteArray> DapQmlModelFullServerList::roleNames() const
{
  return _roleNamesMap;
}

/*-----------------------------------------*/
