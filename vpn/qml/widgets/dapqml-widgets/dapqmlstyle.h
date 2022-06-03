#ifndef DAPQMLSTYLE_H
#define DAPQMLSTYLE_H

/* INCLUDES */
#include <QObject>
#include <QSize>
#include "style/scaled.h"

/****************************************//**
 * @brief Style manager for qml widgets
 *
 * This allows to create CSS-like (and Qss-like) style definitions.
 *
 * @section secDapQmlStyleQss QSS Rules
 * All qss classes and sections must start with dot symbol (.)
 *
 * @section secDapQmlStyleThemes QSS Themes
 * DapQmlStyle also supports themes.
 *
 * To use themes you need to do four things:
 * - Provide qss section with the name starting with symbol \@ (example: <b>.@dark{ }</b>)
 * - Store qss values inside the scope of that theme section (example: <b>.dark{ text-color: "#0004EE"; }</b>)
 * - Call @ref changeCategory providing a theme name (example: <b>DapQmlStyle::changeCategory("dark");</b>)
 * - Now you can access theme values by providing that name starting with symbol \@ inside qss rule value (example: <b>.c-text{ color: "@text-color" }</b>)
 *
 * @code
 * .@dark{
 *   text-color: "#0004EE";
 * }
 *
 * .@white{
 *   text-color: "#CECECE";
 * }
 *
 * .c-text{
 *   color: "@text-color";
 * }
 * @endcode
 *
 * Now if you call <b>DapQmlStyle::changeCategory("dark")</b> and DapQmlStyle::requestRedraw()</b>, qml widget with property <b>qss</b> equals to <b>"c-text"</b> will get color equals to <b>"#0004EE"</b><br>
 * But if you call <b>DapQmlStyle::changeCategory("dark")</B>, color will be equal to <b>"#CECECE"</b>
 *
 *@note feel free to use qproperty-<i>yourFieldName</i> to make DapQmlStyle to call setProperty("yourFieldName", value)
 *
 * @section secDapQmlStyleItem QML Item Control
 * To make QML item being controlled by DapQmlStyle do this:
 * - Define DapQmlStyle as a child item (example: <b>DapQmlStyle {}</b>)
 * - Provide QML item to this class field called "item" (example: <b>DapQmlStyle { item: parent }</b>)
 * - Provide style rules string separated by spaces (example: <b>DapQmlStyle { qss: "c-text font-jost"; item: parent }</b>)
 *
 * @note Class can control items that are not parents. It just treat QML item as a QObject and uses setProperty method to control it values.
 *
 * @ingroup groupDapGuiStyle
 * @date 29.12.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlStyle : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{

  /**
   * @brief List of qss styles joined by space
   * @accessors getQss(), setQss()
   * @example "font-jost font-bold settings-title"
   */
  Q_PROPERTY (QString qss READ getQss WRITE setQss NOTIFY qssChanged)

  /**
   * @brief QML item pointer that this style class is related to
   * @accessors getItem(), setItem()
   */
  Q_PROPERTY (QObject *item READ getItem WRITE setItem NOTIFY itemChanged)

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:

  /// @example "font-jost font-bold settings-title"
  QString m_qss;
  QObject *m_item;

  /**
   * @brief Class instance responsible to scaling features
   * Usualy needed to resize item based on window/screen size
   */
  Style::Scaled m_scaled;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlStyle (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE QString getQss() const;
  Q_INVOKABLE void setQss (const QString &a_newQss);

  Q_INVOKABLE QObject *getItem() const;
  Q_INVOKABLE void setItem (QObject *a_newItem);

  /// @brief window resize event
  /// called by Main.qml form
  Q_INVOKABLE void windowResized (int a_width, int a_height);
  /// force all widgets to redraw
  Q_INVOKABLE void requestRedraw();

  /// @deprecated
  Q_INVOKABLE double centerHor (QObject *a_root, QObject *a_item);
  /// @deprecated
  Q_INVOKABLE double centerVer (QObject *a_root, QObject *a_item);

  /// @brief install qss stylesheet text
  static void setup (const QString &styleSheet);

  /// @brief change qss theme name
  static void changeCategory (const QString &a_category);

  /// @brief reinstall qss stylesheet text
  /// @note this is for debug purposes
  static void update();
  /// @brief send a signal about window resizing
  static void sWindowResized (int a_width, int a_height);
  /// @brief send a signal about redraw request
  static void sRequestRedraw();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  /**
   * @brief QML item redraw requested
   *
   * This signal is sent from static object. @see sRequestRedraw
   */
  void redrawRequested();

  void qssChanged();
  void itemChanged();

  /// @brief window\\screen resized event
  void resized (int a_width, int a_height);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  /// @brief Apply style fields to QML item
  void _applyStyle();
  /// @brief Apply scale to QML item
  void _resized (int a_width, int a_height);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSTYLE_H
