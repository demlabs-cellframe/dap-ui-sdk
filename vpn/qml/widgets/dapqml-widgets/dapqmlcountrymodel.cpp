/* INCLUDES */
#include "dapqmlcountrymodel.h"
#include "style/qsslink.h"
#include "DapDataLocal.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlCountryModel::DapQmlCountryModel(QObject *parent)
  : QAbstractTableModel (parent)
  , m_Countries (DapServersData::m_countryMap.keys())
  , m_checkedIndex(-1)
{
    qDebug() << "DapQmlCountryModel::DapQmlCountryModel" << DapServersData::m_countryMap.keys().size();
    updateCheckedIndex();
}

/********************************************
 * METHODS
 *******************************************/
bool DapQmlCountryModel::isCurrent(const QString &a_name) const
{
//  auto countryCode = DapDataLocal::instance()->getSetting (SETTING_COUNTRY).toString();
//  return DapServersData::m_countryMap[a_name] == countryCode;
  return false;
}

void DapQmlCountryModel::updateCheckedIndex()
{
    auto country = DapDataLocal::instance()->getSetting (SETTING_COUNTRY).toString();
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

/*-----------------------------------------*/
