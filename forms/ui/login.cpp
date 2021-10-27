/* INCLUDES */
#include "login.h"
#include "ui_login.h"
#include "ui/chooseserver.h"
#include <QDebug>
#include <QThread>

/* DEFS */
#define EXPECT_LENGTH (16)
#define MAX_LENGTH (EXPECT_LENGTH + 3)

/* VARS */
static Login *__inst          = nullptr;
static QString s_enteredSerial;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Login::Login (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Login)
{
  /* ui */
  __inst  = this;
  ui->setupUi (this);
  ui->btnChooseSerial->setMaxLength (MAX_LENGTH);
  {
    auto style = ui->btnChooseSerial->edit()->cssStyle();
    style.remove ("noborder");
    style.append (" login-input-border");
    ui->btnChooseSerial->edit()->setCssStyle(style);
  }
#ifdef Q_OS_ANDROID
  ui->btnChooseSerial->setBtnStyle (DapGuiButton::TopMainBottomSub);
  ui->btnChooseSerial->setMainText ("____-____-____-____");
#else
  ui->btnChooseSerial->setInputMask (">NNNN-NNNN-NNNN-NNNN;_");
#endif // Q_OS_ANDROID

//  ui->btnChooseSerial->setInputFocusCallback (cbSerialFocus);
//#ifdef Q_OS_ANDROID
//  ui->btnChooseSerial->setInputCallback (cbSerialText);
//#else
//  ui->btnChooseSerial->setInputMask (">NNNN-NNNN-NNNN-NNNN;_");
//#endif // Q_OS_ANDROID

  /* signals */
  connect (ui->btnChooseServer, &DapGuiButton::clicked,
           this, &Login::sigChooseServer);
  connect (ui->btnChooseSerial, &DapGuiButton::clicked,
           this, &Login::sigChooseSerial);
  connect (ui->btnConnect, &DapGuiPushButton::clicked,
           this, &Login::sigConnect);
  connect (ui->btnObtainNewKey, &DapGuiLabel::clicked,
           this, &Login::sigObtainNewKey);
//  connect (ui->btnChooseSerial, &DapGuiButton::textChanged,
//          this, &Login::sigObtainNewKey);


//  connect(ui->btnChooseSerial, &DapGuiButton::textChanged,[this](QString text){
//      if (ui->btnChooseSerial->mainText().remove("-").count() == MAX_COUNT_CHAR)
//          emit textChangedAndFilledOut(ui->btnChooseSerial->mainText().remove("-"));
//      else if(ui->btnChooseSerial->mainText().remove("-").isEmpty())
//          emit textChangedAndCleaned();
//      else
//          emit slotSerialFillingIncorrect();
//  });
//  connect(ui->btnChooseSerial, &DapGuiButton::textEdited,[this](QString text){
//      Q_UNUSED(text);
//      if (ui->btnChooseSerial->mainText().remove("-").count() == MAX_COUNT_CHAR)
//          emit textEditedAndFilledOut(ui->btnChooseSerial->mainText());
//      else if(ui->btnChooseSerial->mainText().remove("-").isEmpty())
//          emit textEditedAndCleaned();
//      else
//          emit slotSerialFillingIncorrect();
//  });

  connect (ui->btnChooseSerial, &DapGuiButton::textChanged,
           this, &Login::_slotSerialChanged);
  connect (ui->btnChooseSerial, &DapGuiButton::textEdited,
           this, &Login::_slotSerialEdited);
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

void Login::slotSetConnectBtnEnabled (bool enable)
{
//    ui->btnConnect->setDisabled()
  ui->btnConnect->setEnabledCustom (enable);
}
void Login::slotSetConnectBtnText (QString a_text)
{
  //    ui->btnConnect->setDisabled()
  ui->btnConnect->setText (a_text);
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

void Login::_slotSerialChanged (const QString &a_serial)
{
  Q_UNUSED (a_serial);

  auto text = ui->btnChooseSerial->mainText().remove ("-");

  if (text.count() == MAX_COUNT_CHAR)
    emit textChangedAndFilledOut (ui->btnChooseSerial->mainText());
  else if (text.isEmpty())
    emit textChangedAndCleaned();
  else
    emit sigSerialFillingIncorrect();
}

void Login::_slotSerialEdited (const QString &a_serial)
{
  Q_UNUSED (a_serial);

  auto text = ui->btnChooseSerial->mainText().remove ("-");

  if (text.count() == MAX_COUNT_CHAR)
    emit textEditedAndFilledOut (ui->btnChooseSerial->mainText());
  else if (text.isEmpty())
    emit textEditedAndCleaned();
  else
    emit sigSerialFillingIncorrect();
}

/*-----------------------------------------*/
