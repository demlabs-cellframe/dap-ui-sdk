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

  /* signals */
  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &Language::sigReturn,
           Qt::QueuedConnection);
}

Language::~Language()
{
  delete ui;
}

/*-----------------------------------------*/
