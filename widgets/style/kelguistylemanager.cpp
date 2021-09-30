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
static KelGuiStyleManager s_signal (nullptr);

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
    s_styleMap.clear();
    s_styleMap.setup (styleSheet);
  }
};

//typedef QSharedPointer<_GlobalStyleSheet> Gss;
typedef _GlobalStyleSheet Gss;


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiStyleManager::KelGuiStyleManager (QWidget *parent)
  : QObject (parent)
  , m_widget (parent)
{
  if (this != &s_signal)
    connect (&s_signal, &KelGuiStyleManager::forceStyleUpdate,
             this, &KelGuiStyleManager::forcedStyleUpdate);
}

KelGuiStyleManager::KelGuiStyleManager (KelGuiStyleManager &&src)
{
  if (this == &src)
    return;

  setParent (src.m_widget);
  m_cssStyle  = std::move (src.m_cssStyle);
  m_widget    = src.m_widget;
  //src.m_cssStyle.clear();
  src.m_widget  = nullptr;

  if (this != &s_signal)
    connect (&s_signal, &KelGuiStyleManager::forceStyleUpdate,
             this, &KelGuiStyleManager::forcedStyleUpdate);
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
  emit s_signal.forceStyleUpdate();
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
  forcedStyleUpdate();
  emit cssStyleChanged();
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

KelGuiStyleManager &KelGuiStyleManager::_signal()
{
  return s_signal;
}

void KelGuiStyleManager::forcedStyleUpdate()
{
  if(!m_widget)
    return;
  QString s   =
    "#" + m_widget->objectName() +
    "{" + styleByClassName (m_cssStyle) + "}";
  m_widget->setStyleSheet (s);
}

/*-----------------------------------------*/
