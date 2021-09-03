/* INCLUDES */
#include "connection.h"
#include "ui_connection.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Connection::Connection(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Connection)
{
  ui->setupUi(this);
}

Connection::~Connection()
{
  delete ui;
}

/*-----------------------------------------*/
