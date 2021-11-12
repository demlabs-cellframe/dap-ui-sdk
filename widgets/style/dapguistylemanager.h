#ifndef DAPGUISTYLEMANAGER_H
#define DAPGUISTYLEMANAGER_H

/************************************************//**
 * @defgroup groupDapGuiStyle Dap Gui Style
 * Classes responsible for Gui style
 * @ingroup groupDapGuiWidgets
 * @date 07.09.2021
 * @authors Mikhail Shilenko
 ***************************************************/

/* INCLUDES */
#include <QObject>
#include <QStyleOption>
#include <QPainter>

#define DAPGUI_ENABLECSS \
public: \
  Q_PROPERTY (QString cssStyle READ cssStyle WRITE setCssStyle) \
  DapGuiStyleManager __kgsm = DapGuiStyleManager (this); \
  QString cssStyle() const { return __kgsm.cssStyle(); } \
  void setCssStyle (const QString &cssStyle) { __kgsm.setCssStyle (cssStyle); }

#define DAPGUI_ENABLEWIDGETSTYLE \
void paintEvent (QPaintEvent *) override \
{ \
  QStyleOption opt; \
  opt.init (this); \
  QPainter p (this); \
  style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this); \
}

/****************************************//**
 * @brief style manager for widgets
 *
 * Class provide easy way to setup different
 * style classes and get result stylesheet
 * by providing classNames list (setCssStyle)
 * separated by space (' ')
 *
 * ### Class setup
 *
 * @code
 * setCssStyle ("tab content outline font16");
 * @endcode
 *
 * ### Attaching to widget
 *
 * To enable style applying to your widget
 * put define DAPGUI_ENABLECSS inside
 * child class.
 *
 * @code
 * class BoxButton : public QPushButton
 * {
 *   DAPGUI_ENABLECSS
 * public:
 *   explicit Box (QWidget *parent)
 *     : QPushButton (parent) {}
 * }
 * @endcode
 *
 * To enable QWidget styling add define
 * DAPGUI_ENABLEWIDGETSTYLE inside child class
 *
 * @code
 * class Box : public QWidget
 * {
 *   DAPGUI_ENABLECSS
 *   DAPGUI_ENABLEWIDGETSTYLE
 * public:
 *   explicit Box (QWidget *parent)
 *     : QWidget (parent) {}
 * }
 * @endcode
 *
 * ### Css class definitions
 *
 * All css classes is contained inside static
 * map, which contains data parsed from
 * one global css-file provided by the static method
 * DapGuiStyleManager::setupGlobalStyleSheet
 *
 * If you want to start using Css Classes,
 * read and provide correct CSS-file body to
 * DapGuiStyleManager::setupGlobalStyleSheet
 *
 * ### Rules
 *
 * Css class name always starts with dot symbol<br>
 * All other class definition will cause issues.
 *
 * @code
 * ".tab {border: 1px solid black;} .margin28 {margin:28px;}"
 * @endcode
 *
 * @warning Don't use classes definitions without dot
 * symbol at the start of the class name!
 * It will give an undefined behavior.
 *
 * @code
 * DapGuiStyleManager::setupGlobalStyleSheet (".margin28 {margin:28px;}");
 * widget.setCssStyle ("margin28);
 * @endcode
 *
 * @ingroup groupDapGuiStyle
 * @date 07.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiStyleManager : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString cssStyle READ cssStyle WRITE setCssStyle NOTIFY cssStyleChanged)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QString m_cssStyle;
  QWidget *m_widget;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiStyleManager (QWidget *parent);
  DapGuiStyleManager (DapGuiStyleManager &&src);
  virtual ~DapGuiStyleManager();
  /// @}

  /****************************************//**
   * @name STATIC METHODS
   *******************************************/
  /// @{
public:
  static void setupGlobalStyleSheet (
    const QString &styleSheet);
  static void setupTheme (
    const QJsonArray &themesArray,
    const QString &themeName);
  static const QString &themeDir();
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  QString cssStyle() const;
  void setCssStyle (const QString &cssStyle);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void cssStyleChanged();   ///< emited when item change it's style
  void forceStyleUpdate();  ///< emited when global style changed
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected:
  /// get style by class name
  QString styleByClassName (const QString &className);
  /// collect all styles by class array (separated by space)
  QString styleByClassList (const QString &classNameList);
  /// collect all styles by class array
  QString styleByClassList (const QStringList &classNameList);
  /// get singleton signal instance
  DapGuiStyleManager &_signal();
protected slots:
  void forcedStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUISTYLEMANAGER_H
