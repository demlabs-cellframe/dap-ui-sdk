#ifndef KELGUILINEEDIT_H
#define KELGUILINEEDIT_H

/* INCLUDES */
#include <QLineEdit>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief overlap for line edit style
 * @ingroup groupKelGuiWidgets
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiLineEdit : public QLineEdit
{
  Q_OBJECT
  KELGUI_ENABLECSS

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef void (*cbTextEdit) (KelGuiLineEdit *e, QString &preedit, QString &commit, int from, int to);
  typedef void (*cbFocusEvent) (KelGuiLineEdit *e, const Qt::FocusReason &reason);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
public:
  cbTextEdit m_callbackTextEdit;
  cbFocusEvent m_callbackFocusEvent;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiLineEdit (QWidget *parent = nullptr);
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
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{

signals:
  void sigTextEditing();
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
#endif // Q_OS_ANDROID
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUILINEEDIT_H
