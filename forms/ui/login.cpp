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
  /* ui */
  ui->setupUi (this);

  /* signals */
  connect (ui->btnChooseServer, &KelGuiButton::clicked,
           this, &Login::sigChooseServer);
  connect (ui->btnChooseSerial, &KelGuiButton::clicked,
           this, &Login::sigChooseSerial);
  connect (ui->btnConnect, &KelGuiPushButton::clicked,
           this, &Login::sigConnect);
  connect (ui->btnObtainNewKey, &KelGuiLabel::clicked,
           this, &Login::sigObtainNewKey);
}

Login::~Login()
{
  delete ui;
}

/********************************************
 * SLOTS
 *******************************************/

void Login::slotKeyEnable(bool enable)
{
  ui->btnChooseSerial->setVisible (enable);
}

void Login::slotErrorText(QString text)
{
  ui->lError->setText (text);
}

void Login::slotChangeServerName(QString serverName)
{
  ui->btnChooseServer->setMainText (serverName);
}

void Login::slotChangeSerial(QString serialKey)
{
  ui->btnChooseSerial->setMainText (serialKey);
}

/*-----------------------------------------*/
