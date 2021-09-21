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
  QString placeholderText() const;
  void setPlaceholderText(const QString &placeholderText);

  QString plainText() const;
  void setPlainText(const QString &plainText);

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
