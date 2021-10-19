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
}

Language::~Language()
{
  delete ui;
}

/*-----------------------------------------*/
