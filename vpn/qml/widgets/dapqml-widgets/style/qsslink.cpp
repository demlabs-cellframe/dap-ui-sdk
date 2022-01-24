/* INCLUDES */
#include "qsslink.h"

/* NAMESPACE */
namespace Style
{

/* VARS */
static QHash<QString, QssLink> s_categories;
static QString s_currentCategory  = "light";

/* LINKS */
extern void removeMinuses (QString &a_value);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

QssLink::QssLink()
{

}

QssLink::QssLink (const QssLink &a_src)
{
  operator= (a_src);
}

QssLink::QssLink (QssLink &&a_src)
{
  operator= (a_src);
}

QssLink::~QssLink()
{

}

/********************************************
 * METHODS
 *******************************************/

QStringList QssLink::categories()
{
  return s_categories.keys();
}

void QssLink::changeCategory(const QString a_category)
{
  s_currentCategory = a_category;
}

QVariant QssLink::get (QString a_name, QString a_category)
{
  /* set current category if not provided */
  if (a_category.isEmpty())
    a_category  = s_currentCategory;

  /* remove @ symbol */
  if (a_name.at(0) == '@')
    a_name  = a_name.mid (1);

  /* remove minuses */
  removeMinuses (a_name);

  /* check category existence */
  if (!s_categories.contains (a_category))
    return QVariant();

  /* get category */
  const QssLink &cat = s_categories[a_category];

  /* check value existence */
  if (!cat.m_map.contains (a_name))
    return QVariant();

  /* return value */
  return cat.m_map.value (a_name);
}

void QssLink::set (QString a_name, QString a_category, const QVariant &a_value)
{
  /* remove @ symbol */
  if (a_category.at(0) == '@')
    a_category  = a_category.mid (1);

  /* remove minuses */
  removeMinuses (a_name);

  /* create category if not present */
  if (!s_categories.contains (a_category))
    s_categories.insert (a_category, QssLink());

  /* get category */
  QssLink &cat = s_categories[a_category];

  /* set value */
  cat.m_map.insert (a_name, a_value);
}

void QssLink::set(const QString &a_category, const QssItem &a_value)
{
  for (auto it = a_value.cbegin(), en = a_value.cend(); it != en; it++)
    set (it.key(), a_category, it.value());
}

/********************************************
 * OPERATORS
 *******************************************/

QssLink &QssLink::operator = (const QssLink &a_src)
{
  m_map = a_src.m_map;
  return *this;
}

QssLink &QssLink::operator = (QssLink &&a_src)
{
  m_map = std::move (a_src.m_map);
  return *this;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
