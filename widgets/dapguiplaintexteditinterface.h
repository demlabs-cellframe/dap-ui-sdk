#ifndef DAPGUIPLAINTEXTEDITINTERFACE_H
#define DAPGUIPLAINTEXTEDITINTERFACE_H

/* INCLUDES */
#include <QPlainTextEdit>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief overlap for line edit style
 * @ingroup groupDapGuiWidgets
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiPlainTextEditInterface : public QPlainTextEdit
{
  Q_OBJECT
  DAPGUI_ENABLECSS

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef void (*cbTextEdit) (DapGuiPlainTextEditInterface *e, QString &preedit, QString &commit, int from, int to);
  typedef void (*cbFocusEvent) (DapGuiPlainTextEditInterface *e, const Qt::FocusReason &reason);
  typedef bool (*cbKeyEvent) (DapGuiPlainTextEditInterface *e, QKeyEvent *event);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
public:
  cbTextEdit m_callbackTextEdit;
  cbFocusEvent m_callbackFocusEvent;
  cbFocusEvent m_callbackUnfocusEvent;
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
public:
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
protected:
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
