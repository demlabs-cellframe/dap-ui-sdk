/* INCLUDES */
#include "login.h"
#include "ui_login.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Login::Login(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Login)
{
  ui->setupUi(this);
}

Login::~Login()
{
  delete ui;
}

/*-----------------------------------------*/
