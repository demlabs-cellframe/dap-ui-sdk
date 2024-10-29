#ifndef DAPGUILINEEDITNATIVE_H
#define DAPGUILINEEDITNATIVE_H

/* INCLUDES */
#include <QLineEdit>
#include <QDebug>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief overlap for line edit style and
 * events
 *
 * Features (Descktop):
 * - Css Style
 * - Event callbacks (cbKeyEvent)
 *
 * Features (Android)
 * - Event callbacks (cbTextEdit, cbFocusEvent)
 * - Hide/Show anchors
 * - Text editing signal (sigTextEditing)
 *
 * @ingroup groupDapGuiWidgets
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/
class DapGuiLineEditNative : public QLineEdit
{
  Q_OBJECT
  DAPGUI_ENABLECSS

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef void (*cbTextEdit) (DapGuiLineEditNative *e, QString &preedit, QString &commit, int from, int to);
  typedef void (*cbFocusEvent) (DapGuiLineEditNative *e, const Qt::FocusReason &reason);
  typedef bool (*cbKeyEvent) (DapGuiLineEditNative *e, QKeyEvent *event);
  typedef bool (*cbInputMethodEvent) (DapGuiLineEditNative *widget, QEvent *event);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
public:
  cbInputMethodEvent m_callbackEvent; ///< called in event
  cbTextEdit m_callbackTextEdit;      ///< called in inputMethodEvent
  cbFocusEvent m_callbackFocusEvent;  ///< called in focusInEvent
  cbKeyEvent m_callbackKeyEvent;      ///< called in keyPressEvent
  bool m_hideAnchor;                  ///< hide's android anchors
  QString commiting;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiLineEditNative (QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  cbTextEdit callbackTextEdit() const;
  void setCallbackTextEdit(cbTextEdit cb);

  cbFocusEvent callbackFocusEvent() const;
  void setCallbackFocusEvent(cbFocusEvent newCallbackFocusEvent);

  cbKeyEvent callbackKeyEvent() const;
  void setCallbackKeyEvent(cbKeyEvent newCallbackKeyEvent);

  cbInputMethodEvent callbackEvent() const;
  void setCallbackEvent(cbInputMethodEvent newCallbackInputMethodEvent);
  /// @}
  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
  bool hideAnchor() const;
  void setHideAnchor(bool newHideAnchor);

signals:
  void sigTextEditing();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotUnfocus();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  void keyPressEvent(QKeyEvent *event) override;
#ifdef Q_OS_ANDROID_UNUSED
  void inputMethodEvent(QInputMethodEvent *e) override;
  void focusInEvent(QFocusEvent *e) override;
public:
  QVariant inputMethodQuery (Qt::InputMethodQuery a_imq) const override;
  Q_INVOKABLE QVariant inputMethodQuery(Qt::InputMethodQuery property, QVariant argument) const;
#endif // Q_OS_ANDROID
  /// @}

};

/*-----------------------------------------*/
#endif // DAPGUILINEEDITNATIVE_H
