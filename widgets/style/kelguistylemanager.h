#ifndef KELGUISTYLEMANAGER_H
#define KELGUISTYLEMANAGER_H

/************************************************//**
 * @defgroup groupKelGuiStyle Kel Gui Style
 * Classes responsible for Gui style
 * @ingroup groupKelGuiWidgets
 * @date 07.09.2021
 * @authors Mikhail Shilenko
 ***************************************************/

/* INCLUDES */
#include <QObject>
#include <QMutexLocker>

#define KELGUI_ENABLECSSSTYLE \
void applyStyle() override \
{ setStyleSheet (styleByClassName (m_cssStyle)); }

#define KELGUI_ENABLEWIDGETSTYLE \
void paintEvent (QPaintEvent *) override \
{ \
  QStyleOption opt; \
  opt.init (this); \
  QPainter p (this); \
  style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this); \
}

/****************************************//**
 * @brief style manager for widgets
 * Class provide easy way to setup different
 * style classes and get result stylesheet
 * by providing classNames list (setCssStyle)
 * separated by space (' ')
 *
 * @code
 * setCssStyle(
 *   ".tab .content,"
 *   ".outline .font16");
 * @endcode
 *
 * To enable style autoapplying put
 * define KELGUI_ENABLECSSSTYLE inside
 * child class.
 *
 * @code
 * class Box : public KelGuiStyleManager, QPushButton
 * {
 *   KELGUI_ENABLECSSSTYLE
 * public:
 *   explicit Box (QWidget *parent)
 * : QPushButton (parent) {}
 * }
 * @endcode
 *
 * To enable QWidget styling add define
 * KELGUI_ENABLEWIDGETSTYLE inside child class
 *
 * @code
 * class Box : public KelGuiStyleManager, QWidget
 * {
 *   KELGUI_ENABLECSSSTYLE
 *   KELGUI_ENABLEWIDGETSTYLE
 * public:
 *   explicit Box (QWidget *parent)
 * : QWidget (parent) {}
 * }
 * @endcode
 *
 * @ingroup groupKelGuiStyle
 * @date 07.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiStyleManager : public QObject
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
  KelGuiStyleManager (QWidget *parent);
  KelGuiStyleManager (KelGuiStyleManager &&src);
  virtual ~KelGuiStyleManager();
  /// @}

  /****************************************//**
   * @name STATIC METHODS
   *******************************************/
  /// @{
public:
  static void setupGlobalStyleSheet (
    const QString &styleSheet,
    const float &DPI);
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
  void cssStyleChanged();
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
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUISTYLEMANAGER_H
