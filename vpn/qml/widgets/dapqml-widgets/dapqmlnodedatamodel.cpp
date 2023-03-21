/* INCLUDES */
#include "dapqmlnodedatamodel.h"
#include "style/qsslink.h"
#include <QDebug>

#define RoleName (Qt::UserRole + 1)
#define RoleSubText (Qt::UserRole + 2)
#define RoleChecked (Qt::UserRole + 3)
#define RoleHash (Qt::UserRole + 4)


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlNodeDataModel::DapQmlNodeDataModel(QObject *parent)
  : QAbstractTableModel (parent)
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
        if (index(k, 0).data(RoleHash) == checkedName)
        {
            int oldIndex = m_checkedIndex;
            m_checkedIndex = k;
            emit dataChanged(index(oldIndex, 0), index(oldIndex, 0));
            emit dataChanged(index(k, 0), index(k, 0));
            break;
        }
}

void DapQmlNodeDataModel::fill(const QList<QStringList> &rowsData)
{
    qDebug() << "fill model" << rowsData;
    beginResetModel();
    for (const auto& item : rowsData)
        m_nodeData << (QStringList() << item[0] << item[1] << item[2]);
    endResetModel();
    emit dataChanged(index(0, 0), index(m_nodeData.length(), 1));
}

void DapQmlNodeDataModel::fill(const QStringList &rowsData)
{
    qDebug() << "fill model" << rowsData;
    beginResetModel();
    for (const auto& item : rowsData)
        m_nodeData << (QStringList() << item << item << "");
    endResetModel();
    emit dataChanged(index(0, 0), index(m_nodeData.length(), 1));
}

void DapQmlNodeDataModel::fill(const QMap<QString, QString> &rowsData)
{
    qDebug() << "fill model" << rowsData;
    beginResetModel();
    for (const auto& key : rowsData.keys())
        m_nodeData << (QStringList() << key << key << rowsData.value(key));
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
  if (!index.isValid() ||
      !(role == RoleName || role == RoleSubText || role == RoleChecked || role == RoleHash))
    return QVariant();


  QStringList item = m_nodeData.value(index.row());
  if (role == RoleName)
  {
    if (item.length() > 1)
        return item.at(1);
    return "";
  }
  if (role == RoleSubText)
  {
    if (item.length() > 2)
        return item.at(2);
    return "";
  }
  if (role == RoleChecked)
  {
    return index.row() == m_checkedIndex;
  }
  if (role == RoleHash)
  {
    if (item.length() > 1)
        return item.at(0);
    return "";
  }
  return QVariant();
}

QHash<int, QByteArray> DapQmlNodeDataModel::roleNames() const
{
  QHash<int, QByteArray> names;
  names[RoleName]       = "name";
  names[RoleSubText]    = "subText";
  names[RoleChecked]    = "checked";
  names[RoleHash]       = "hash";
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
