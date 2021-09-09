/* INCLUDES */
#include "connection.h"
#include "ui_connection.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Connection::Connection (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Connection)
{
  ui->setupUi (this);
}

Connection::~Connection()
{
  delete ui;
}

/********************************************
 * OVERRIDE
 *******************************************/

void Connection::restartUi()
{
  BASEFORM_RESTARTUI_ROUTINE (Ui::Connection);
}

/*-----------------------------------------*/
