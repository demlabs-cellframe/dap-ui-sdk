#ifndef DAPGUILINEEDIT_H
#define DAPGUILINEEDIT_H

/* INCLUDES */
#include <QLineEdit>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief overlap for line edit style
 * @ingroup groupDapGuiWidgets
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiLineEdit : public QLineEdit
{
  Q_OBJECT
  DAPGUI_ENABLECSS

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef void (*cbTextEdit) (DapGuiLineEdit *e, QString &preedit, QString &commit, int from, int to);
  typedef void (*cbFocusEvent) (DapGuiLineEdit *e, const Qt::FocusReason &reason);
  typedef bool (*cbKeyEvent) (DapGuiLineEdit *e, QKeyEvent *event);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
public:
  cbTextEdit m_callbackTextEdit;
  cbFocusEvent m_callbackFocusEvent;
  cbKeyEvent m_callbackKeyEvent;
  bool m_hideAnchor;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiLineEdit (QWidget *parent = nullptr);
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
#ifdef Q_OS_ANDROID
  void inputMethodEvent(QInputMethodEvent *e) override;
  void focusInEvent(QFocusEvent *e) override;
public:
  QVariant inputMethodQuery (Qt::InputMethodQuery a_imq) const override;
  Q_INVOKABLE QVariant inputMethodQuery(Qt::InputMethodQuery property, QVariant argument) const;
#endif // Q_OS_ANDROID
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUILINEEDIT_H
