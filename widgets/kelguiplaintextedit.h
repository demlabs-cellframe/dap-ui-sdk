#ifndef KELGUIPLAINTEXTEDIT_H
#define KELGUIPLAINTEXTEDIT_H

/* INCLUDES */
#include <QWidget>
#include <QPlainTextEdit>

/* DEFS */
namespace Ui { class KelGuiPlainTextEditUI; };

/****************************************//**
 * @brief framed plaintextedit
 * @ingroup groupKelGuiWidgets
 * @date 24.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiPlainTextEdit : public QWidget
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
  Ui::KelGuiPlainTextEditUI *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiPlainTextEdit(QWidget *parent = 0);
  ~KelGuiPlainTextEdit();
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

  QPlainTextEdit *editWidget();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void textChanged(); ///< text is changed
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIPLAINTEXTEDIT_H
