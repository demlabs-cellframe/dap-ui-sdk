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
#ifdef Q_OS_ANDROID
  ui->btnChooseSerial->setBtnStyle (KelGuiButton::TopMainBottomSub);
  ui->btnChooseSerial->setMainText ("____-____-____-____");
#endif

  /* signals */
  connect (ui->btnChooseServer, &KelGuiButton::clicked,
           this, &Login::sigChooseServer);
  connect (ui->btnChooseSerial, &KelGuiButton::clicked,
           this, &Login::sigChooseSerial);
  connect (ui->btnConnect, &KelGuiPushButton::clicked,
           this, &Login::sigConnect);
  connect (ui->btnObtainNewKey, &KelGuiLabel::clicked,
           this, &Login::sigObtainNewKey);
//  connect (ui->btnChooseSerial, &KelGuiButton::textChanged,
//          this, &Login::sigObtainNewKey);


//  connect(ui->btnChooseSerial, &KelGuiButton::textChanged,[this](QString text){
//      if (ui->btnChooseSerial->mainText().remove("-").count() == MAX_COUNT_CHAR)
//          emit textChangedAndFilledOut(ui->btnChooseSerial->mainText().remove("-"));
//      else if(ui->btnChooseSerial->mainText().remove("-").isEmpty())
//          emit textChangedAndCleaned();
//      else
//          emit slotSerialFillingIncorrect();
//  });
//  connect(ui->btnChooseSerial, &KelGuiButton::textEdited,[this](QString text){
//      Q_UNUSED(text);
//      if (ui->btnChooseSerial->mainText().remove("-").count() == MAX_COUNT_CHAR)
//          emit textEditedAndFilledOut(ui->btnChooseSerial->mainText());
//      else if(ui->btnChooseSerial->mainText().remove("-").isEmpty())
//          emit textEditedAndCleaned();
//      else
//          emit slotSerialFillingIncorrect();
//  });

  connect (ui->btnChooseSerial, &KelGuiButton::textChanged,
           this, &Login::slotSerialChanged);
  connect (ui->btnChooseSerial, &KelGuiButton::textEdited,
           this, &Login::slotSerialEdited);
}

Login::~Login()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

QString Login::getSerialNumber()
{
  return ui->btnChooseSerial->mainText();
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
//    ui->btnConnect->setDisabled()
  ui->btnConnect->setEnabledCustom (enable);
}
void Login::slotSetConnectBtnText(QString a_text)
{
    //    ui->btnConnect->setDisabled()
    ui->btnConnect->setText(a_text);
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

void Login::slotSerialChanged(const QString &a_serial)
{
  Q_UNUSED(a_serial);

  auto text = ui->btnChooseSerial->mainText().remove("-");

  if (text.count() == MAX_COUNT_CHAR)
    emit textChangedAndFilledOut(ui->btnChooseSerial->mainText());
  else if(text.isEmpty())
    emit textChangedAndCleaned();
  else
    emit slotSerialFillingIncorrect();
}

void Login::slotSerialEdited(const QString &a_serial)
{
  Q_UNUSED(a_serial);

  auto text = ui->btnChooseSerial->mainText().remove("-");

  if (text.count() == MAX_COUNT_CHAR)
    emit textEditedAndFilledOut(ui->btnChooseSerial->mainText());
  else if(text.isEmpty())
    emit textEditedAndCleaned();
  else
    emit slotSerialFillingIncorrect();
}

/*-----------------------------------------*/
