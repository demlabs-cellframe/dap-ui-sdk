#ifndef DAPGUIPLAINTEXTEDITINTERFACE_H
#define DAPGUIPLAINTEXTEDITINTERFACE_H

/* INCLUDES */
#include <QPlainTextEdit>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief overlap for line edit style
 *
 * Features Desktop:
 * - Event callbacks (cbKeyEvent)
 *
 * Features Android:
 * - Event callbacks (cbTextEdit, cbFocusEvent)
 *
 * @ingroup groupDapGuiWidgets
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiPlainTextEditInterface : public QPlainTextEdit
{
  Q_OBJECT
  DAPGUI_ENABLECSS

  friend class DapGuiPlainTextEdit;

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef bool (*cbTextEdit) (DapGuiPlainTextEditInterface *e, QString &preedit, QString &commit, int from, int to);
  typedef void (*cbFocusEvent) (DapGuiPlainTextEditInterface *e, const Qt::FocusReason &reason);
  typedef bool (*cbKeyEvent) (DapGuiPlainTextEditInterface *e, QKeyEvent *event);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  /**
   * @brief callback on inputMethodEvent
   * @note only on Android
   * @accessors %callbackTextEdit(), setCallbackTextEdit()
   */
  cbTextEdit m_callbackTextEdit;
  /**
   * @brief callback on focusInEvent
   * @note only on Android
   * @accessors %callbackFocusEvent(), setCallbackFocusEvent()
   */
  cbFocusEvent m_callbackFocusEvent;
  /**
   * @brief callback on focusOutEvent
   * @note only on Android
   * @accessors %callbackUnfocusEvent(), setCallbackUnfocusEvent()
   */
  cbFocusEvent m_callbackUnfocusEvent;
  /**
   * @brief callback on keyPressEvent
   * @accessors %callbackKeyEvent(), setCallbackKeyEvent()
   */
  cbKeyEvent m_callbackKeyEvent;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiPlainTextEditInterface (QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
protected:
  cbTextEdit callbackTextEdit() const;
  void setCallbackTextEdit(cbTextEdit cb);

  cbFocusEvent callbackFocusEvent() const;
  void setCallbackFocusEvent(cbFocusEvent newCallbackFocusEvent);

  cbKeyEvent callbackKeyEvent() const;
  void setCallbackKeyEvent(cbKeyEvent newCallbackKeyEvent);

  cbFocusEvent callbackUnfocusEvent() const;
  void setCallbackUnfocusEvent(cbFocusEvent newCallbackUnfocusEvent);

  void unfocus();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
private:
  void keyPressEvent(QKeyEvent *event) override;
#ifdef Q_OS_ANDROID
  void inputMethodEvent(QInputMethodEvent *e) override;
  void focusInEvent(QFocusEvent *e) override;
  void focusOutEvent(QFocusEvent *e) override;
#endif // Q_OS_ANDROID
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIPLAINTEXTEDITINTERFACE_H
