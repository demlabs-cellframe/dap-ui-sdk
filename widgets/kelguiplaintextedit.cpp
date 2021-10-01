/* INCLUDES */
#include "kelguiplaintextedit.h"
#include "ui_kelguiplaintextedit.h"
#include <QStyle>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiPlainTextEdit::KelGuiPlainTextEdit (QWidget *parent) :
    QWidget (parent),
    ui(new Ui::KelGuiPlainTextEditUI)
{
  ui->setupUi(this);

  /* signals */

  connect(ui->kgpteuPlainTextEdit, &QPlainTextEdit::textChanged,
          [=] ()
  {
      style()->polish(ui->kgpteuPlainTextEdit);
      emit textChanged();
    });
}

KelGuiPlainTextEdit::~KelGuiPlainTextEdit()
{
  delete ui;
}

/********************************************
 * PUBLIC METHODS
 *******************************************/

Qt::ScrollBarPolicy KelGuiPlainTextEdit::verticalScrollBarPolicy() const
{
  return ui->kgpteuPlainTextEdit->verticalScrollBarPolicy();
}

void KelGuiPlainTextEdit::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setVerticalScrollBarPolicy (a_policy);
}

Qt::ScrollBarPolicy KelGuiPlainTextEdit::horizontalScrollBarPolicy() const
{
  return ui->kgpteuPlainTextEdit->horizontalScrollBarPolicy();
}

void KelGuiPlainTextEdit::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setHorizontalScrollBarPolicy (a_policy);
}

QAbstractScrollArea::SizeAdjustPolicy KelGuiPlainTextEdit::sizeAdjustPolicy() const
{
  return ui->kgpteuPlainTextEdit->sizeAdjustPolicy();
}

void KelGuiPlainTextEdit::setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setSizeAdjustPolicy (a_policy);
}

QString KelGuiPlainTextEdit::placeholderText() const
{
  return ui->kgpteuPlainTextEdit->placeholderText();
}

void KelGuiPlainTextEdit::setPlaceholderText(const QString &placeholderText)
{
  ui->kgpteuPlainTextEdit->setPlaceholderText (placeholderText);
}

QString KelGuiPlainTextEdit::plainText() const
{
  return ui->kgpteuPlainTextEdit->toPlainText();
}

void KelGuiPlainTextEdit::setPlainText(const QString &plainText)
{
  ui->kgpteuPlainTextEdit->setPlainText(plainText);
}

void KelGuiPlainTextEdit::setTextCursor(const QTextCursor &a_cursor)
{
  ui->kgpteuPlainTextEdit->setTextCursor (a_cursor);
}

QTextCursor KelGuiPlainTextEdit::textCursor() const
{
  return ui->kgpteuPlainTextEdit->textCursor();
}

QPlainTextEdit *KelGuiPlainTextEdit::editWidget()
{
  return ui->kgpteuPlainTextEdit;
}

/*-----------------------------------------*/
