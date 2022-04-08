#ifndef DAPQMLSERIALKEYINPUT_H
#define DAPQMLSERIALKEYINPUT_H

/* INCLUDES */
#include <QQuickItem>

/****************************************//**
 * @brief serial key input field widget
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
  typedef bool (*cbInputMethodEvent) (DapQmlSerialKeyInput *widget, QEvent *event);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  cbInputMethodEvent m_callbackEvent; ///< called at event
  QString m_commiting;
  QObject *m_qmlItem;
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
  const QString &commiting() const;
  void setCommiting(const QString &newCommiting);

  QString text() const;
  void setText (const QString &a_text);

  cbInputMethodEvent callbackEvent() const;
  void setCallbackEvent(cbInputMethodEvent newCallbackInputMethodEvent);
  Q_INVOKABLE void setup (QObject *a_qmlItem);
  Q_INVOKABLE void setFocus();
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
