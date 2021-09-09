/* INCLUDES */
#include "statistics.h"
#include "ui_statistics.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Statistics::Statistics (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Statistics)
{
  ui->setupUi (this);
}

Statistics::~Statistics()
{
  delete ui;
}

/*-----------------------------------------*/
