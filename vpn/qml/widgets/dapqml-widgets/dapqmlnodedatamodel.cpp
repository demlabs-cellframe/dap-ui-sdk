/* INCLUDES */
#include "dapqmlnodedatamodel.h"
#include "style/qsslink.h"
#include <QDebug>

#define RoleName (Qt::UserRole + 1)
#define RoleChecked (Qt::UserRole + 2)


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlNodeDataModel::DapQmlNodeDataModel(QObject *parent)
  : QAbstractTableModel (parent)
  , m_nodeData (QStringList())
  , m_checkedIndex(-1)
{
    updateCheckedIndex("");
}

/********************************************
 * METHODS
 *******************************************/
void DapQmlNodeDataModel::updateCheckedIndex(const QString& checkedName)
{
    for (int k = 0; k < m_nodeData.size(); k++)
        if (index(k, 0).data(0) == checkedName)
        {
            int oldIndex = m_checkedIndex;
            m_checkedIndex = k;
            emit dataChanged(index(oldIndex, 0), index(oldIndex, 0));
            emit dataChanged(index(k, 0), index(k, 0));
            break;
        }
}

void DapQmlNodeDataModel::fill(const QStringList &rowsData)
{
    qDebug() << "fill model" << rowsData;
    beginResetModel();
    m_nodeData = rowsData;
    endResetModel();
    emit dataChanged(index(0, 0), index(m_nodeData.length(), 1));
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlNodeDataModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return m_nodeData.size();
}

int DapQmlNodeDataModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlNodeDataModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || !(role == RoleName || role == RoleChecked))
    return QVariant();


  auto item = m_nodeData.value (index.row());
  if (role == RoleName)
  {
    if (item.length() > 1)
      item = item.at(0).toUpper() + item.mid(1);
    return item;
  }
  if (role == RoleChecked)
  {
    return index.row() == m_checkedIndex;
  }
  return QVariant();
}

QHash<int, QByteArray> DapQmlNodeDataModel::roleNames() const
{
  QHash<int, QByteArray> names;

  names[RoleName]       = "name";
  names[RoleChecked]    = "checked";

  return names;
}



DapQmlNodeDataSortFilterProxyModel::DapQmlNodeDataSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_filter("")
    , m_model(new DapQmlNodeDataModel(parent))
{
    setSourceModel(m_model);
}

void DapQmlNodeDataSortFilterProxyModel::updateCheckedIndex(QString checkedName)
{
    DapQmlNodeDataModel* model = (DapQmlNodeDataModel*) sourceModel();
    model->updateCheckedIndex(checkedName);
}

void DapQmlNodeDataSortFilterProxyModel::setRowFilter(QString str)
{
    m_filter = str;
    invalidateFilter();
}

bool DapQmlNodeDataSortFilterProxyModel::filterAcceptsRow(
        int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    return (sourceModel()->data(index0, 0).toString().contains(m_filter, Qt::CaseInsensitive));
}


/*-----------------------------------------*/
