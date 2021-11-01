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
  QMetaObject::invokeMethod(ui->scrollArea, &LanguageModel::slotSetup, Qt::QueuedConnection);

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &Language::sigReturn,
           Qt::QueuedConnection);
  connect (ui->scrollArea, &LanguageModel::sigSelect,
           this, &Language::sigSelect,
           Qt::QueuedConnection);
}

Language::~Language()
{
  delete ui;
}

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

LanguageModel *Language::model()
{
  return ui->scrollArea;
}

/*-----------------------------------------*/
