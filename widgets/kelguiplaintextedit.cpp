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
//  connect(ui->kgpteuPlainTextEdit, &QPlainTextEdit::textChanged,
//          this, &KelGuiPlainTextEdit::textChanged);

  /* style */

}

KelGuiPlainTextEdit::~KelGuiPlainTextEdit()
{
  delete ui;
}

/********************************************
 * PUBLIC METHODS
 *******************************************/

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

QPlainTextEdit *KelGuiPlainTextEdit::editWidget()
{
  return ui->kgpteuPlainTextEdit;
}

/*-----------------------------------------*/
