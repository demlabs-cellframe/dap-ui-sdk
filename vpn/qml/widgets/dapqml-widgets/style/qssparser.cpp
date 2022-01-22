/* INCLUDES */
#include "qssparser.h"
#include "qssmap.h"
#include "scaled.h"
#include "qsslink.h"
#include "qssvalue.h"
#include <QStringList>
#include <QDebug>

/* NAMESPACE */
namespace Style
{

/* DEFS */
typedef QString::const_iterator it;
typedef void (*callback) (it &i);

/* LINKS */
static void none (it &i);
static void naming (it &i);
static void body (it &i);
extern QHash<QString, Style::QssItem> s_map;

/* VARS */
static QStringList names;
static QString currentName, currentBody;
static callback method  = none;
static int index        = 0;

/********************************************
 * METHODS
 *******************************************/

void QssParser::perform (const QString &styleSheet)
{
  /* vars */
  QString simpleSheet = styleSheet;

  /* remove lines */
  simpleSheet =
    simpleSheet
    .replace ('\r', " ")
    .replace ('\n', " ")
    //.replace (' ', "")
    .replace ('\t', "");

  /* split by combo of new lines and dots */
  for (auto i = simpleSheet.cbegin(),
       e = simpleSheet.cend();
       i != e;
       i++, index++)
    method (i);
}

/********************************************
 * STATIC METHODS
 *******************************************/

void none (it &i)
{
  /* start naming new item */
  if (*i == '.')
    {
      /* change mode to naming */
      method = naming;
      return;
    }

  /* start naming new item */
  if (*i == '{')
    {
      /* change mode to body */
      method = body;
      return;
    }
}

void naming (it &i)
{
  /* store name and start new one */
  if ((*i == ' ') || (*i == '{') || (*i == ','))
    {
      /* store name into list */
      names  += currentName;
      method = (*i == ' ') ? none : body;
      currentName.clear();
      return;
    }

  /* store name letter */
  currentName += *i;
}

void body (it &i)
{
  /* body finished */
  if (*i == '}')
    {
      /* if got atleast on keyname */
      if (!names.isEmpty())
        {
          /* store into map */
          //styleMap.insert (names, currentBody);

          /* parse body */
          auto lines  = QssValue::parse (currentBody);

          /* form an item */
          QssItem item;
          qDebug() << "new item: " << names;
          for (auto it = lines.cbegin(), en = lines.cend(); it != en; it++)
            {
              auto value  = it->asVariant();
              qDebug() << "field[" << it->field() << "]:[" << value << "]";
              item.insert (it->field(), value);
            }

          /* store */
          for (auto it = names.cbegin(), en = names.cend(); it != en; it++)
            {
              /* get name */
              QString name  = *it;

              /* store as link */
              if (name.at(0) == '@')
                QssLink::set(name, item);

              /* store as item */
              else
                s_map.insert (name, item);
            }
        }

      /* clear names and body */
      names.clear();
      currentName.clear();
      currentBody.clear();

      /* change mode to none */
      method = none;
      return;
    }

  /* store body letter */
  currentBody += *i;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
