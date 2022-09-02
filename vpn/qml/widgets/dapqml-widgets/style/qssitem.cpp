#include "qssitem.h"

/* NAMESPACE */
namespace DapStyle
{

/********************************************
 * METHODS
 *******************************************/

QssItemMap::iterator QssItem::insert(const QString &a_key, const QVariant &a_value)
{
  return m_map.insert(a_key, a_value);
}

bool QssItem::remove(const QString &a_key)
{
  return m_map.remove (a_key);
}

QssItemMap::iterator QssItem::erase(QssItemMap::iterator &it)
{
  return m_map.erase (it);
}



QssItemMap::iterator QssItem::begin()
{
  return m_map.begin();
}

QssItemMap::iterator QssItem::end()
{
  return m_map.end();
}

QssItemMap::const_iterator QssItem::cbegin() const
{
  return m_map.cbegin();
}

QssItemMap::const_iterator QssItem::cend() const
{
  return m_map.cend();
}



int QssItem::size() const
{
  return m_map.size();
}



QList<QString> QssItem::keys() const
{
  return m_map.keys();
}

QList<QVariant> QssItem::values() const
{
  return m_map.values();
}



QVariant QssItem::value(const QString &a_key) const
{
  return m_map.value (a_key);
}

QString QssItem::key(const QVariant &a_value) const
{
  return m_map.key (a_value);
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
