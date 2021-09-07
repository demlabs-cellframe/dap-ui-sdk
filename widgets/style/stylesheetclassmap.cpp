/* INCLUDES */
#include "stylesheetclassmap.h"
#include "stylesheetclassparser.h"

/* NAMESPACE */
namespace StyleSheet
{

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

//ClassMap::ClassMap()
//{

//}

/********************************************
 * METHODS
 *******************************************/

void ClassMap::setup (const QString &styleSheet, const float &DPI)
{
  StyleSheet::ClassParser (styleSheet, *this, DPI);
}

void ClassMap::clear()
{
  m_allKeys.clear();
  m_allKeys.reserve (2);

  m_items.clear();
  m_items.reserve (2);
}

void ClassMap::insert (const QString &key, const QString &style)
{
  /* try to find */
  auto it = find (key);

  /* if found, store */
  if (isValid (it))
    return it->styles.append (style);

  /* if not found, create */
  m_items.append ({{key}, {style}});
  m_allKeys.append (key);
}

void ClassMap::insert (const QStringList &keys, const QString &style)
{
  m_items.append ({{keys}, {style}});
  m_allKeys.append (keys);
}

const QStringList &ClassMap::keys() const
{
  return m_allKeys;
}

const QStringList &ClassMap::value (const QString &key) const
{
  /* vars */
  static QStringList result;

  /* cycle thru existing items */
  for (auto j = m_items.cbegin(), k = m_items.cend(); j != k; j++)
    {
      /* check if item contains key */
      if (j->keys.contains (key))

        // store into list
        result << j->styles;
    }

  return result;
}

const QStringList &ClassMap::values (const QStringList &keys) const
{
  /* vars */
  static QStringList result;

  /* cycle trhu provided keys */
  for (auto i = keys.cbegin(), e = keys.cend(); i != e; i++)
    {
      /* cycle thru existing items */
      for (auto j = m_items.cbegin(), k = m_items.cend(); j != k; j++)
        {
          /* check if item contains one of the provided keys */
          if (j->keys.contains (*i))

            // store into list
            result << j->styles;
        }
    }

  return result;
}

/********************************************
 * OPERATORS
 *******************************************/

const QStringList &ClassMap::operator[] (const QString &key) const
{
  return value (key);
}

const QStringList &ClassMap::operator[] (const QStringList &keys) const
{
  return values (keys);
}

/********************************************
 * PROTECTED METHODS
 *******************************************/

ClassMap::Iterator ClassMap::find (const QString &key)
{
  for (auto i = m_items.begin(), e = m_items.end(); i != e; i++)
    {
      if (!i->keys.contains (key))
        continue;
      return i;
    }

  return m_items.end();
}

ClassMap::ConstIterator ClassMap::find (const QString &key) const
{
  for (auto i = m_items.cbegin(), e = m_items.cend(); i != e; i++)
    {
      if (!i->keys.contains (key))
        continue;
      return i;
    }

  return m_items.cend();
}

bool ClassMap::isValid (ClassMap::Iterator &it) const
{
  return it != m_items.end();
}

bool ClassMap::isValid (ConstIterator &it) const
{
  return it != m_items.cend();
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
