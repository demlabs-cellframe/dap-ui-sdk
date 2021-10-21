#ifndef DAPGUIPLAINTEXTEDIT_H
#define DAPGUIPLAINTEXTEDIT_H

/* INCLUDES */
#include <QWidget>
#include <QPlainTextEdit>

/* DEFS */
namespace Ui { class DapGuiPlainTextEditUI; };
class DapGuiPlainTextEditInterface;

/****************************************//**
 * @brief framed plaintextedit overlap
 *
 * This widget presented as frame around inerhitted
 * from QPlainTextEdit widget DapGuiPlainTextEditInterface
 *
 * @ingroup groupDapGuiWidgets
 * @date 24.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiPlainTextEdit : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY(Qt::ScrollBarPolicy verticalScrollBarPolicy READ verticalScrollBarPolicy WRITE setVerticalScrollBarPolicy)
  Q_PROPERTY(Qt::ScrollBarPolicy horizontalScrollBarPolicy READ horizontalScrollBarPolicy WRITE setHorizontalScrollBarPolicy)
  Q_PROPERTY(QAbstractScrollArea::SizeAdjustPolicy sizeAdjustPolicy READ sizeAdjustPolicy WRITE setSizeAdjustPolicy)
  Q_PROPERTY (QString placeholderText   READ placeholderText  WRITE setPlaceholderText)
  Q_PROPERTY (QString plainText         READ plainText        WRITE setPlainText)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::DapGuiPlainTextEditUI *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiPlainTextEdit(QWidget *parent = 0);
  ~DapGuiPlainTextEdit();
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  Qt::ScrollBarPolicy verticalScrollBarPolicy() const;
  void setVerticalScrollBarPolicy (Qt::ScrollBarPolicy a_policy);

  Qt::ScrollBarPolicy horizontalScrollBarPolicy() const;
  void setHorizontalScrollBarPolicy (Qt::ScrollBarPolicy a_policy);

  QAbstractScrollArea::SizeAdjustPolicy sizeAdjustPolicy() const;
  void setSizeAdjustPolicy (QAbstractScrollArea::SizeAdjustPolicy a_policy);

  QString placeholderText() const;
  void setPlaceholderText(const QString &placeholderText);

  QString plainText() const;
  void setPlainText(const QString &plainText);

  void setTextCursor(const QTextCursor &a_cursor);
  QTextCursor textCursor() const;

  QPlainTextEdit *editWidget() const;
  void unfocus();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void textChanged(); ///< text is changed
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected:
  static void _cbTextEdit (DapGuiPlainTextEditInterface *e, QString &preedit, QString &commit, int from, int to);
  static void _cbFocusEvent (DapGuiPlainTextEditInterface *e, const Qt::FocusReason &reason);
  static void _cbUnfocusEvent (DapGuiPlainTextEditInterface *e, const Qt::FocusReason &reason);
  static bool _cbKeyEvent (DapGuiPlainTextEditInterface *e, QKeyEvent *event);
  bool eventFilter(QObject* obj, QEvent* event) override;

  void _updateStyle();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIPLAINTEXTEDIT_H
