/* INCLUDES */
#include "stylesheetclassparser.h"
#include <QStringList>

/* NAMESPACE */
namespace StyleSheet
{

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ClassParser::ClassParser (
  const QString &styleSheet,
  ClassMap &styleMap)
  : styleMap (styleMap),
    method (none),        ///< none as default callback
    index (0)
{
  /* vars */
  QString simpleSheet = styleSheet;

  /* remove lines */
  simpleSheet =
    simpleSheet
    .replace ('\n', " ")
//    .replace ('{', "")
//    .replace ('}', "")
//    .replace ('\n', "")
    .replace ('\t', "");

  /* split by combo of new lines and dots */
  for (auto i = simpleSheet.cbegin(),
       e = simpleSheet.cend();
       i != e;
       i++, index++)
    method (this, i);
}

/********************************************
 * STATIC METHODS
 *******************************************/

//    .tab_content .tab_top,
//    .button_top
//    {
//      font-size: 14pt;
//    }

void ClassParser::none (ClassParser *ssc, it &i)
{
  /* start naming new item */
  if (*i == '.')
    {
      /* change mode to naming */
      ssc->method = naming;
      return;
    }

  /* start naming new item */
  if (*i == '{')
    {
      /* change mode to body */
      ssc->method = body;
      return;
    }
}

void ClassParser::naming (ClassParser *ssc, it &i)
{
  /* store name and start new one */
  if ((*i == ' ') || (*i == '{') || (*i == ','))
    {
      /* store name into list */
      ssc->names  += ssc->currentName;
      ssc->method = (*i == ' ') ? none : body;
      ssc->currentName.clear();
      return;
    }

  /* store name letter */
  ssc->currentName += *i;
}

void ClassParser::body (ClassParser *ssc, it &i)
{
  /* body finished */
  if (*i == '}')
    {
      /* if got atleast on keyname */
      if (!ssc->names.isEmpty())
        {
          /* store into map */
          ssc->styleMap.insert (ssc->names, ssc->currentBody);
        }

      /* clear names and body */
      ssc->names.clear();
      ssc->currentName.clear();
      ssc->currentBody.clear();

      /* change mode to none */
      ssc->method = none;
      return;
    }

  /* store body letter */
  ssc->currentBody += *i;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
