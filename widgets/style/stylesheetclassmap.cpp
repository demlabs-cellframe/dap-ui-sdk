/* INCLUDES */
#include "stylesheetclassmap.h"
#include "stylesheetclassparser.h"
#include <QJsonObject>

/* NAMESPACE */
namespace StyleSheet
{

/********************************************
 * METHODS
 *******************************************/

void ClassMap::setup (const QString &styleSheet)
{
  StyleSheet::ClassParser (styleSheet, *this);
}

void ClassMap::patch (const QJsonArray &array)
{
  for (auto i = array.constBegin(), e = array.constEnd(); i != e; i++)
    {
      /* get jobject */
      auto j = (*i).toObject();

      /* read into structure */
      struct
      {
        QString name;
        QString replace;
        QString append;
      } item =
      {
        j.value("name").toString(),
        j.value("replace").toString(),
        j.value("append").toString()
      };

      /* check */
      if (!m_allKeys.contains(item.name))
        continue;

      /* get origin */
      QStringList origin  = values ({item.name});
      bool canUpdate      = false;

      /* if append */
      if (!item.append.isEmpty())
        {
          QStringList append  = values ({item.append});
          origin.append (append);
          canUpdate           = true;
        }

      /* if replace */
      else if (!item.replace.isEmpty())
        {
          QStringList replace = values ({item.replace});
          origin              = replace;
          canUpdate           = true;
        }

      /* update */
      if (canUpdate)
        {
          for (auto i = m_items.begin(), e = m_items.end(); i != e; i++)
            {
              /* skip */
              if (!i->keys.contains (item.name))
                continue;

              /* store */
              i->styles = origin;
            }
        }
    }
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
//  /* vars */
//  static QStringList result;

//  /* cycle thru existing items */
//  for (auto j = m_items.cbegin(), k = m_items.cend(); j != k; j++)
//    {
//      /* check if item contains key */
//      if (j->keys.contains (key))

//        // store into list
//        result << j->styles;
//    }

//  return result;
  QStringList keys = key.split (' ');
  return values (keys);
}

const QStringList &ClassMap::values (const QStringList &keys) const
{
  /* vars */
  static QStringList result;
  result.clear();

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
