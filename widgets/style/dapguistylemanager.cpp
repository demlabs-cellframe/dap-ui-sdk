/* INCLUDES */
#include "dapguistylemanager.h"
//#include <QFile>
#include <QWidget>
#include <QMutexLocker>
#include "stylesheetclassmap.h"
#include "stylesheetclassparser.h"
//#include <QClipboard>
//#include <QApplication>
#include <QJsonArray>
#include <QJsonObject>

/* VARS */
static StyleSheet::ClassMap s_styleMap;
static QMutex s_mutex;
static DapGuiStyleManager s_signal (nullptr);
static QString s_themeDir = "light";

/* DEFS */

namespace StyleSheet
{

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

  void patch (const QJsonArray &themesArray, const QString &themeDir)
  {
    s_styleMap.patch (themesArray, themeDir);
  }

  QStringList keys()
  {
    return s_styleMap.keys();
  }
};

};

//typedef QSharedPointer<_GlobalStyleSheet> Gss;
using Gss = StyleSheet::_GlobalStyleSheet;


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

void DapGuiStyleManager::setupTheme(
  const QJsonArray &themesArray,
  const QString &themeName)
{
  bool success = false;

  for (auto i = themesArray.constBegin(), e = themesArray.constEnd(); i != e; i++)
    {
      auto j = (*i).toObject();

      if (j.value("name").toString() != themeName
          && j.value("dir").toString() != themeName)
        continue;

      success = true;
      Gss().patch (j.value ("patch").toArray(), j.value ("dir").toString());
    }

  if (!success)
    {
      auto j = (*themesArray.constBegin()).toObject();
      Gss().patch (j.value ("patch").toArray(), j.value ("dir").toString());
    }

  emit s_signal.forceStyleUpdate();
}

const QString &DapGuiStyleManager::themeDir()
{
  return s_themeDir;
}

/********************************************
 * PUBLIC METHODS
 *******************************************/




/****************************************//**
 * @property DapGuiStyleManager::cssStyle
 * @brief css style classes separated by space
 * @accessors %cssStyle(), %setCssStyle()
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

QStringList DapGuiStyleManager::classList()
{
  return Gss().keys();
}

DapGuiStyleManager &DapGuiStyleManager::_signal()
{
  return s_signal;
}

void DapGuiStyleManager_setThemeDir(const QString &a_themeDir)
{
  s_themeDir  = a_themeDir;
}

void DapGuiStyleManager::forcedStyleUpdate()
{
  if(!m_widget)
    return;

  auto style    = styleByClassName (m_cssStyle);
  auto objName  = m_widget->objectName();

  if(!style.contains("background"))
    style += "background-color: rgba(0,0,0,0);";

  QString s   =
    "#" + objName +
    "{" + style + "}";

  if (s == m_widget->styleSheet())
    return;

  m_widget->setStyleSheet (s);
}

/*-----------------------------------------*/
