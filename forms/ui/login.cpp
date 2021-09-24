/* INCLUDES */
#include "login.h"
#include "ui_login.h"
#include "ui/chooseserver.h"

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

void Login::slotKeyEnable (bool enable)
{
  ui->btnChooseSerial->setVisible (enable);
}

void Login::slotSetConnectBtnEnabled(bool enable)
{
  ui->btnConnect->setEnabled (enable);
}

void Login::slotErrorText (QString text, ErrorColor color)
{
  ui->lStatus->setText (text);
  ui->lStatus->setCssStyle (
    (color == ErrorColor::ERRCOL_RED)
    ? "font13 red_error lato normal"
    : "font13 darkblue lato normal"
  );
}

void Login::slotChangeServerName (QString serverName)
{
  ui->btnChooseServer->setMainText (serverName);
}

void Login::slotChangeSerial (QString serialKey)
{
  ui->btnChooseSerial->setMainText (serialKey);
}

void Login::slotServerChooserActivate()
{
  ui->btnChooseServer->setEnabled (true);
}

void Login::slotClearStatusLable()
{
  ui->lStatus->clear();
}

void Login::setSerialNumber (const QString &a_serial)
{
  ui->btnChooseSerial->setMainText (a_serial);
}

QString Login::getSerialNumber()
{
  return ui->btnChooseSerial->mainText();
}

/*-----------------------------------------*/
