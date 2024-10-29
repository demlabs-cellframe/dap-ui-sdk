/* INCLUDES */
#include "qssmap.h"
#include "qssparser.h"
#include <QMutex>

/* NAMESPACE */
namespace DapStyle
{

/* VARS */
QHash<QString, DapStyle::QssItem> s_map;
static QMutex m_mapMutex;

/********************************************
 * METHODS
 *******************************************/

void QssMap::setup(const QString &styleSheet)
{
  QssParser::perform (styleSheet);
}

QssItem *QssMap::item(const QString &a_name)
{
  static QssItem s_item;
  QMutexLocker l(&m_mapMutex);

  if (!s_map.contains(a_name))
  {
    s_item  = QssItem();
    return &s_item;
  }

  return &s_map[a_name];
}

QList<QssItem *> QssMap::items(const QString &a_names)
{
  return items (a_names.split(' '));
}

QList<QssItem *> QssMap::items(const QStringList &a_names)
{
  QList<QssItem *> result;

  for (auto i = a_names.cbegin(), e = a_names.cend(); i != e; i++)
    result += item (*i);

  return result;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
