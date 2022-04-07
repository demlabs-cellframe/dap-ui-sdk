#ifndef DAPQMLSERIALKEYINPUT_H
#define DAPQMLSERIALKEYINPUT_H

/* INCLUDES */
#include <QQuickItem>

/****************************************//**
 * @brief serial key input field widget
 * @date 08.04.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlSerialKeyInput : public QQuickItem
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef bool (*cbInputMethodEvent) (DapQmlSerialKeyInput *widget, QEvent *event);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  cbInputMethodEvent m_callbackEvent; ///< called at event
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlSerialKeyInput (QObject *a_parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  cbInputMethodEvent callbackEvent() const;
  void setCallbackEvent(cbInputMethodEvent newCallbackInputMethodEvent);
protected:
  void touchEvent(QTouchEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void inputMethodEvent(QInputMethodEvent *event) override;
  //bool event(QEvent *event) override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSERIALKEYINPUT_H
