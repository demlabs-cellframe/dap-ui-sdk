/* INCLUDES */
#include "login.h"
#include "ui_login.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Login::Login (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Login)
{
  ui->setupUi (this);
}

Login::~Login()
{
  delete ui;
}

/********************************************
 * OVERRIDE
 *******************************************/

void Login::restartUi()
{
  BASEFORM_RESTARTUI_ROUTINE (Ui::Login);
}

/*-----------------------------------------*/
