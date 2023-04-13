/* INCLUDES */
#include "dapqmlcountrymodel.h"
#include "style/qsslink.h"
#include "DapServerList.h"
#include "DapDataLocal.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlCountryModel::DapQmlCountryModel(QObject *parent)
  : QAbstractTableModel (parent)
  , m_Countries (DapAbstractServerList::countryMap().keys())
  , m_checkedIndex (-1)
{
    updateCheckedIndex();
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlCountryModel::updateCheckedIndex()
{
    auto country = DapDataLocal::instance()->getSetting (COUNTRY_NAME).toString();
    for (int k = 0; k < m_Countries.size(); k++)
        if (index(k, 0).data(0) == country)
        {
            int oldIndex = m_checkedIndex;
            m_checkedIndex = k;
            emit dataChanged(index(oldIndex, 0), index(oldIndex, 0));
            emit dataChanged(index(k, 0), index(k, 0));
            break;
        }
}

bool DapQmlCountryModel::countryExist()
{
#ifndef BRAND_RISEVPN
    auto country = DapDataLocal::instance()->getSetting (COUNTRY_NAME).toString();
    return !country.isNull() && !country.isEmpty();
#else
    return true;
#endif
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlCountryModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return m_Countries.size();
}

int DapQmlCountryModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlCountryModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || !(role == 0 || role == 1))
    return QVariant();

  auto item = m_Countries.value (index.row());
  if (role == 0)
  {
    if (item.length() > 1)
      item = item.at(0).toUpper() + item.mid(1);
    return item;
  }
  if (role == 1)
  {
    return index.row() == m_checkedIndex;
  }
  return QVariant();
}

QHash<int, QByteArray> DapQmlCountryModel::roleNames() const
{
  QHash<int, QByteArray> names;

  names[0] = "name";
  names[1] = "checked";

  return names;
}



DapQmlCountrySortFilterProxyModel::DapQmlCountrySortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_filter("")
    , m_model(new DapQmlCountryModel(parent))
{
    setSourceModel(m_model);
}

void DapQmlCountrySortFilterProxyModel::updateCheckedIndex()
{
//    DapQmlCountryModel* model = (DapQmlCountryModel*) sourceModel();
//    model->updateCheckedIndex();
  m_model->updateCheckedIndex();
}

void DapQmlCountrySortFilterProxyModel::setRowFilter (const QString &a_filter)
{
    m_filter = a_filter;
    invalidateFilter();
}

bool DapQmlCountrySortFilterProxyModel::filterAcceptsRow(
        int sourceRow, const QModelIndex &sourceParent) const
{
    //QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    //return (sourceModel()->data(index0, 0).toString().startsWith(m_filter, Qt::CaseInsensitive));
  auto index  = m_model->index (sourceRow, 0, sourceParent);
  auto name   = m_model->data (index, 0).toString();
  return name.startsWith (m_filter, Qt::CaseInsensitive);
}


/*-----------------------------------------*/
