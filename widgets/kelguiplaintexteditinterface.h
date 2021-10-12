#ifndef KELGUIPLAINTEXTEDITINTERFACE_H
#define KELGUIPLAINTEXTEDITINTERFACE_H

/* INCLUDES */
#include <QPlainTextEdit>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief overlap for line edit style
 * @ingroup groupKelGuiWidgets
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiPlainTextEditInterface : public QPlainTextEdit
{
  Q_OBJECT
  KELGUI_ENABLECSS

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef void (*cbTextEdit) (KelGuiPlainTextEditInterface *e, QString &preedit, QString &commit, int from, int to);
  typedef void (*cbFocusEvent) (KelGuiPlainTextEditInterface *e, const Qt::FocusReason &reason);
  typedef bool (*cbKeyEvent) (KelGuiPlainTextEditInterface *e, QKeyEvent *event);
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
  KelGuiPlainTextEditInterface (QWidget *parent = nullptr);
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
#endif // KELGUIPLAINTEXTEDITINTERFACE_H
