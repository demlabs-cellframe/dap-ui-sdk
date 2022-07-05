/* INCLUDES */
#include "dapqmlthememodel.h"
#include "style/qsslink.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlThemeModel::DapQmlThemeModel(QObject *parent)
  : QAbstractTableModel (parent)
  , m_themes (Style::QssLink::categories())
{

}

/********************************************
 * METHODS
 *******************************************/

QString DapQmlThemeModel::currentCategory() const
{
  auto cc = Style::QssLink::currentCategory();
  return cc.at(0).toUpper() + cc.mid(1);
}

bool DapQmlThemeModel::isCurrent(const QString &a_name) const
{
  auto compare = currentCategory();
  return compare == a_name;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlThemeModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return m_themes.size();
}

int DapQmlThemeModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlThemeModel::data(const QModelIndex &index, int role) const
{
  Q_UNUSED(role)

  if (!index.isValid() || role != 0)
    return QVariant();

  auto item = m_themes.value (index.row());
//  if (item.length() > 1)
//    item    = item.at(0).toUpper() + item.mid(1);

  return item;
}

QHash<int, QByteArray> DapQmlThemeModel::roleNames() const
{
  QHash<int, QByteArray> names;

  names[0] = "name";

  return names;
}

/*-----------------------------------------*/
