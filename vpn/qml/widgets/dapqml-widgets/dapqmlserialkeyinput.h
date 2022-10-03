#ifndef DAPQMLSERIALKEYINPUT_H
#define DAPQMLSERIALKEYINPUT_H

/* INCLUDES */
#include <QQuickItem>

/****************************************//**
 * @brief serial key input field filter bridge
 *
 * This class used to provide bridge connection
 * between QML Item Input field and simple
 * event filter as a callback
 *
 * Usualy this helps to control Serial Key input
 * on different platfroms (Desktop/Android)
 *
 * To start working as a event filter:
 * -Create instance of this class
 * -Execute "setup" method providing a Input QML Item
 * -Set a callback for event filtering
 *
 * After this every time QML Item gets a event
 * callback will be called providing this class pointer
 * and event pointer, as if this was a simple
 * QObject event filter
 *
 * @note setup can only called once. if called again - behavior is undefined
 *
 * @date 08.04.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlSerialKeyInput : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString text READ text WRITE setText NOTIFY textChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// @brief callback for event filtering
  typedef bool (*cbInputMethodEvent) (DapQmlSerialKeyInput *widget, QEvent *event);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  cbInputMethodEvent m_callbackEvent; ///< called at event filter
  QString m_commiting;                ///< commiting string for android
  QObject *m_qmlItem;                 ///< pointer to attached qml item
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlSerialKeyInput (QObject *a_parent = nullptr);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void textChanged();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// @brief get commiting string
  /// @note for android only
  const QString &commiting() const;
  /// @brief set commiting string
  /// @note for android only
  void setCommiting(const QString &newCommiting);

  /// @brief get input field text
  QString text() const;
  /// @brief set input field text
  void setText (const QString &a_text);

  /// @brief get attached qml item
  QObject *qmlItem() const;

  /// @brief get installed callback
  Q_INVOKABLE cbInputMethodEvent callbackEvent() const;
  /// @brief install a callback for event filter
  Q_INVOKABLE void setCallbackEvent(cbInputMethodEvent newCallbackInputMethodEvent);
  /// @brief attach qml item
  Q_INVOKABLE void setup (QObject *a_qmlItem);
  /// @brief force focus on input field
  Q_INVOKABLE void setFocus();
  /// @brief simulate backspace press behavior
  Q_INVOKABLE bool sendBackspaceEvent();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSERIALKEYINPUT_H
