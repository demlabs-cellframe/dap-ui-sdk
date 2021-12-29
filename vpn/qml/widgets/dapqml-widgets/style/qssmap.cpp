/* INCLUDES */
#include "qssmap.h"

/* NAMESPACE */
namespace Style
{

/********************************************
 * METHODS
 *******************************************/

void QssMap::setup(const QString &styleSheet)
{

}

void QssMap::patch(const QJsonArray &array, const QString &themeDir)
{

}

const QssItem &QssMap::item(const QString &a_name)
{
  static const QssItem s_item;

  if (!m_map.contains(a_name))
    return s_item;

  return m_map[a_name];
}

QList<const QssItem &> &QssMap::items(const QString &a_names)
{
  return items (a_names.split(' '));
}

QList<const QssItem &> &QssMap::items(const QStringList &a_names)
{
  QList<const QssItem &> result;

  for (auto i = a_names.cbegin(), e = a_names.cend(); i != e; i++)
    result += item (*i);

  return result;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
