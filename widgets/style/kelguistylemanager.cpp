/* INCLUDES */
#include "kelguistylemanager.h"
#include <QMap>
#include <QTextStream>

/* VARS */
static QMap<QString, QString> s_styleMap;
static QMutex s_mutex;

/* DEFS */
class _GlobalStyleSheet : public QMutexLocker
{
public:
  _GlobalStyleSheet() : QMutexLocker(&s_mutex) {}

  /// get style sheet by class name
  QString operator[] (const QString &className)
  {
    auto value = s_styleMap.value (className);
    return value
        .replace('{',"")
        .replace('}',"")
        .replace('\n',"")
        .replace('\t',"");
  }

  /// setup global style sheet
  void set (const QString &styleSheet, const float &DPI)
  {
    struct Item
    {
      enum ItemType
      {
        Class,  ///< class (.)
        Name,   ///< name (#)
      };
    };

//    .tab_content .tab_top,
//    .button_top
//    {
//      font-size: 14pt;
//    }

    QStringList array, names;
    int index = 0, inside = false, naming = false;
    QString simpleSheet = styleSheet, currentName;

    /* remove lines */
    simpleSheet = simpleSheet.replace('\n',"");

//    /* split by combo of new lines and dots */
//    for(auto i = styleSheet.cbegin(), e = styleSheet.cend(); i != e; i++, index++)
//      {
//        if(naming)
//          {
//            /* ignore symbols */
//            if (*i == ',')
//              continue;

//            /* store name and start new one */
//            if (*i == ' ')
//              {
//                names   += currentName;
//                naming  = false;
//                currentName.clear();
//                continue;
//              }

//            /* start naming new item */
//            if (naming == false && *i == '.')
//              {
//                naming = true;
//                continue;
//              }

//            /* store name letter */
//            currentName += *i;
//          }

//        if(inside == false)
//          {
//            if (*i == '\n')
//              {
//                newLine = index;
//                continue;
//              }

//            if (*i == '.' && ((newLine == index) || (newLine == index - 1)))
//              {
//                inside = true;
//                naming = true;
//              }
//          }
//      }

//    /* get all classes (by dot) */
//    for(auto i = array.cbegin(), e = array.cend(); i != e; i++)
//      {
//        auto &item = *i;

//      }

//    /* split by dot */
//    auto list  = styleSheet.split ('.');
//    styleSheet

//    /* cycle thru every class */
//    foreach (auto &item, list)
//      {
//        auto index  = item.indexOf ('\n');
//        if (index == -1)
//          continue;
//        auto name   = item.mid (0, index);
//        auto body   = item.mid (index);
//        s_styleMap.insert (name, body);
//      }
  }
};

typedef QSharedPointer<_GlobalStyleSheet> Gss;


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiStyleManager::KelGuiStyleManager(QObject *parent)
  : QObject (parent)
{

}
