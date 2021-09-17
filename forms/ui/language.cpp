/* INCLUDES */
#include "language.h"
#include "ui_language.h"

#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Language::Language (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Language)
{
  ui->setupUi (this);
  QTimer::singleShot (0, ui->scrollArea, &LanguageModel::slotSetup);
}

Language::~Language()
{
  delete ui;
}

/*-----------------------------------------*/
