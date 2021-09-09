/* INCLUDES */
#include "kelguistylemanager.h"
//#include <QFile>
#include <QWidget>
#include <QMutexLocker>
#include "stylesheetclassmap.h"
#include "stylesheetclassparser.h"
//#include <QClipboard>
//#include <QApplication>

/* VARS */
static StyleSheet::ClassMap s_styleMap;
static QMutex s_mutex;

/* DEFS */

/// class wrap over mutex and give acces to map
class _GlobalStyleSheet : public QMutexLocker
{
public:
  _GlobalStyleSheet() : QMutexLocker (&s_mutex) {}

  /// get style sheet by class name
  QString operator[] (const QString &className)
  {
    auto value = s_styleMap.value (className);
    return value.join ('\n');
  }

  /// get style sheet by class name
  QString operator[] (const QStringList &classNameList)
  {
    auto value = s_styleMap.values (classNameList);
    return value.join ('\n');
  }

  /// setup global style sheet
  void set (const QString &styleSheet)
  {
    s_styleMap.setup (styleSheet);
  }
};

//typedef QSharedPointer<_GlobalStyleSheet> Gss;
typedef _GlobalStyleSheet Gss;


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiStyleManager::KelGuiStyleManager (QWidget *parent)
  : m_widget (parent)
{

}

KelGuiStyleManager::KelGuiStyleManager (KelGuiStyleManager &&src)
{
  if (this == &src)
    return;

  m_cssStyle  = src.m_cssStyle;
  m_widget    = src.m_widget;
  src.m_cssStyle.clear();
  src.m_widget  = nullptr;
}

KelGuiStyleManager::~KelGuiStyleManager()
{

}

/********************************************
 * STATIC METHODS
 *******************************************/

void KelGuiStyleManager::setupGlobalStyleSheet (const QString &styleSheet)
{
  Gss().set (styleSheet);
}

/********************************************
 * PUBLIC METHODS
 *******************************************/

QString KelGuiStyleManager::cssStyle() const
{
  return m_cssStyle;
}

void KelGuiStyleManager::setCssStyle (const QString &cssStyle)
{
  m_cssStyle  = cssStyle;
  QString s   =
    "#" + m_widget->objectName() +
    "{" + styleByClassName (m_cssStyle) + "}";
  m_widget->setStyleSheet (s);
//  QApplication::clipboard()->setText (s);
//  QFile f ("/home/user/log.log");
//  f.open (QIODevice::WriteOnly | QIODevice::Truncate);
//  f.write (s.toStdString().c_str());
}

/********************************************
 * PROTECTED METHODS
 *******************************************/

QString KelGuiStyleManager::styleByClassName (const QString &className)
{
  return Gss()[className];
}

QString KelGuiStyleManager::styleByClassList (const QString &classNameList)
{
  return Gss()[classNameList];
}

QString KelGuiStyleManager::styleByClassList (const QStringList &classNameList)
{
  return Gss()[classNameList];
}

/*-----------------------------------------*/
