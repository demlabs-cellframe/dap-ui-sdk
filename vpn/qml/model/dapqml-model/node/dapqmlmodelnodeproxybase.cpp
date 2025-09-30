/* INCLUDES */

#include "dapqmlmodelnodeproxybase.h"

#include <QDebug>

/* DEFINES */

#define DEBUGINFO qDebug() << (QString ("NodeOrderList::") + __func__)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelNodeProxyBase::DapQmlModelNodeProxyBase (QObject *a_parent)
  : QSortFilterProxyModel (a_parent)
  , _indexMapCounter (0)
{
  setSourceModel (dynamic_cast<QAbstractItemModel *> (a_parent));
  setBridge (dynamic_cast<Bridge *> (a_parent));
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelNodeProxyBase::setRowFilter (const QString &a_filter)
{
  m_filter  = a_filter;
  _indexMap.clear();
  _indexMapCounter  = 0;
  invalidateFilter();
  _printFilteredResult();
}

DapQmlModelNodeProxyBase::Bridge *DapQmlModelNodeProxyBase::bridge() const
{
  return m_bridge;
}

void DapQmlModelNodeProxyBase::setBridge (Bridge *a_bridge)
{
  m_bridge  = a_bridge;
  invalidateFilter();
}

int DapQmlModelNodeProxyBase::currentIndex() const
{
  if (m_bridge == nullptr)
    return -1;
  int sourceIndex = m_bridge->currentIndex();
  if (sourceIndex <= -1)
    return -1;
  if (sourceIndex >= m_bridge->size())
    return -1;
  int proxyIndex  =
      (!m_bridge->isIndexed())
      ? sourceIndex
      : _indexMap.value (sourceIndex, -1); //mapFromSource (index (sourceIndex, 0)).row();
  return proxyIndex;
}

void DapQmlModelNodeProxyBase::setCurrentIndex (int a_value)
{
  if (m_bridge == nullptr)
    return;

  int sourceIndex = (!m_bridge->isIndexed())
      ? a_value
      : _indexMap.key (a_value, -1); //mapToSource (index (a_value, 0)).row();

  if (m_bridge->setCurrentIndex (sourceIndex))
    emit sigCurrentIndexChanged();
}

void DapQmlModelNodeProxyBase::_printFilteredResult() const
{
  int size  = rowCount();

  qDebug() << "DapQmlListModuleProxyModel::_printFilteredResult" << size << m_filter;

  for (int i = 0; i < size; i++)
    qDebug() << data (index (i, 0), 9 ).toString()
             << data (index (i, 0), 10).toString();
}

/********************************************
 * OVERRIDE
 *******************************************/

bool DapQmlModelNodeProxyBase::filterAcceptsRow (int a_row, const QModelIndex &) const
{
  if (m_bridge == nullptr)
    return false;

  if (m_filter.isEmpty())
    return true;

  bool match  = m_bridge->filterAcceptsRow (a_row, m_filter);
  int size = m_bridge->size();
  if (match){
    if (size < _indexMapCounter){
      _indexMapCounter = 0;
    }
    _indexMap.insert (a_row, _indexMapCounter++);
  }
  return match;
}

/*-----------------------------------------*/
