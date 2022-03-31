/* INCLUDES */
#include "qssmap.h"
#include "qssparser.h"
#include <QMutex>

/* NAMESPACE */
namespace Style
{

/* VARS */
QHash<QString, Style::QssItem> s_map;
static QMutex m_mapMutex;

/********************************************
 * METHODS
 *******************************************/

void QssMap::setup(const QString &styleSheet)
{
  QssParser::perform (styleSheet);
}

void QssMap::patch(const QJsonArray &array, const QString &themeDir)
{

}

const QssItem *QssMap::item(const QString &a_name)
{
  static const QssItem s_item;
  QMutexLocker l(&m_mapMutex);

  if (!s_map.contains(a_name))
    return &s_item;

  return &s_map[a_name];
}

QList<const QssItem *> QssMap::items(const QString &a_names)
{
  return items (a_names.split(' '));
}

QList<const QssItem *> QssMap::items(const QStringList &a_names)
{
  QList<const QssItem *> result;

  for (auto i = a_names.cbegin(), e = a_names.cend(); i != e; i++)
    result += item (*i);

  return result;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
