/* INCLUDES */
#include "dapguistylemanager.h"
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
static DapGuiStyleManager s_signal (nullptr);

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

DapGuiStyleManager::DapGuiStyleManager (QWidget *parent)
  : QObject (parent)
  , m_widget (parent)
{
  if (this != &s_signal)
    connect (&s_signal, &DapGuiStyleManager::forceStyleUpdate,
             this, &DapGuiStyleManager::forcedStyleUpdate);
}

DapGuiStyleManager::DapGuiStyleManager (DapGuiStyleManager &&src)
{
  if (this == &src)
    return;

  setParent (src.m_widget);
  m_cssStyle  = std::move (src.m_cssStyle);
  m_widget    = src.m_widget;
  //src.m_cssStyle.clear();
  src.m_widget  = nullptr;

  if (this != &s_signal)
    connect (&s_signal, &DapGuiStyleManager::forceStyleUpdate,
             this, &DapGuiStyleManager::forcedStyleUpdate);
}

DapGuiStyleManager::~DapGuiStyleManager()
{

}

/********************************************
 * STATIC METHODS
 *******************************************/

void DapGuiStyleManager::setupGlobalStyleSheet (const QString &styleSheet)
{
  Gss().set (styleSheet);
  emit s_signal.forceStyleUpdate();
}

/********************************************
 * PUBLIC METHODS
 *******************************************/

QString DapGuiStyleManager::cssStyle() const
{
  return m_cssStyle;
}

void DapGuiStyleManager::setCssStyle (const QString &cssStyle)
{
  m_cssStyle  = cssStyle;
  forcedStyleUpdate();
  emit cssStyleChanged();
}

/********************************************
 * PROTECTED METHODS
 *******************************************/

QString DapGuiStyleManager::styleByClassName (const QString &className)
{
  return Gss()[className];
}

QString DapGuiStyleManager::styleByClassList (const QString &classNameList)
{
  return Gss()[classNameList];
}

QString DapGuiStyleManager::styleByClassList (const QStringList &classNameList)
{
  return Gss()[classNameList];
}

DapGuiStyleManager &DapGuiStyleManager::_signal()
{
  return s_signal;
}

void DapGuiStyleManager::forcedStyleUpdate()
{
  if(!m_widget)
    return;

  auto style  = styleByClassName (m_cssStyle);

  if(!style.contains("background"))
    {
      //m_widget->setAttribute(Qt::WA_NoSystemBackground);
      //m_widget->setAttribute(Qt::WA_TranslucentBackground);
      //m_widget->setAttribute(Qt::WA_PaintOnScreen);
      //m_widget->setAttribute(Qt::WA_TransparentForMouseEvents);
      style += "background-color: rgba(0,0,0,0);";
    }

  QString s   =
    "#" + m_widget->objectName() +
    "{" + style + "}";

  m_widget->setStyleSheet (s);
}

/*-----------------------------------------*/
