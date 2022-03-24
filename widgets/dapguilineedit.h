#ifndef DAPGUILINEEDIT_H
#define DAPGUILINEEDIT_H

/* INCLUDES */
#include <QLineEdit>
#include <QDebug>

#ifdef Q_OS_ANDROID
#define USE_QLABEL
#endif

#ifdef USE_QLABEL
#include <QLabel>
#endif
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
#ifdef USE_QLABEL
class DapGuiLineEdit : public QLabel
#else
class DapGuiLineEdit : public QLineEdit
#endif
{
  Q_OBJECT
#ifdef USE_QLABEL
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged USER true)
#endif
  DAPGUI_ENABLECSS

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef void (*cbTextEdit) (DapGuiLineEdit *e, QString &preedit, QString &commit, int from, int to);
  typedef void (*cbFocusEvent) (DapGuiLineEdit *e, const Qt::FocusReason &reason);
  typedef bool (*cbKeyEvent) (DapGuiLineEdit *e, QKeyEvent *event);
  typedef bool (*cbInputMethodEvent) (DapGuiLineEdit *widget, QEvent *event);
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
#ifndef USE_QLABEL
  void keyPressEvent(QKeyEvent *event) override;
#endif
#ifdef Q_OS_ANDROID_UNUSED
  void inputMethodEvent(QInputMethodEvent *e) override;
  void focusInEvent(QFocusEvent *e) override;
public:
  QVariant inputMethodQuery (Qt::InputMethodQuery a_imq) const override;
  Q_INVOKABLE QVariant inputMethodQuery(Qt::InputMethodQuery property, QVariant argument) const;
#endif // Q_OS_ANDROID
  /// @}

#ifdef USE_QLABEL
public:
  int cursorPosition()                  { return 0; }
  void setCursorPosition(int position)  { Q_UNUSED(position) }
  int selectionLength()                 { return 0; }
  void insert(QString text)             { setText(text); }
  int maxLength()                       { return 0; }
  void setMaxLength(int lenght)         { Q_UNUSED(lenght) }
  void setFrame(bool frame)             { Q_UNUSED(frame) }
  void setInputMask(QString mask)       { Q_UNUSED(mask) }
  void setPlaceholderText(QString text) { setText(text); }
  void setText(QString a_text);

signals:
  void textChanged(QString text);
  void textEdited(QString text);

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void showEvent(QShowEvent *event) override;
  void inputMethodEvent(QInputMethodEvent *event) override;
  bool event(QEvent *event) override;
#endif

};

/*-----------------------------------------*/
#endif // DAPGUILINEEDIT_H
