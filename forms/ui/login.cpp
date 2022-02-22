/* INCLUDES */
#include "login.h"
#include "ui_login.h"
#include "ui/chooseserver.h"
#include <QDebug>
#include <QRegExp>
#include <QThread>

/* DEFS */
#define EXPECT_LENGTH (16)
#define MAX_LENGTH (EXPECT_LENGTH + 3)

/* VARS */
static Login *__inst          = nullptr;
static QString s_enteredSerial;

void formatSerialKeyLine(QString &text)
{
    int textLength = text.length();
    if (textLength >= 12)
        text.insert(12, "-");
    if (textLength >= 8)
        text.insert(8, "-");
    if (textLength >= 4)
        text.insert(4, "-");
}

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
  //ui->btnChooseSerial->setInputMask (">NNNN-NNNN-NNNN-NNNN;");
  ui->btnChooseSerial->setPlaceholderText ("____-____-____-____");
#endif // Q_OS_ANDROID

  ui->btnChooseSerial->setCallbackKeyEvent(_cbKeyEvent);
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

  connect (ui->btnChooseSerial, &DapGuiButton::textChanged,[this](const QString &a_serial)
  {
     QString t = a_serial;
     t = t.remove("-").remove(" ").toUpper();
     formatSerialKeyLine(t);
     ui->btnChooseSerial->textChangedSignalLock(true);
     ui->btnChooseSerial->insert(t);
     ui->btnChooseSerial->textChangedSignalLock(false);
     _slotSerialChanged(a_serial);
  });
//  connect (ui->btnChooseSerial, &DapGuiButton::textEdited,
//           this, &Login::_slotSerialEdited);
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

void Login::slotRetranslated()
{
  ui->btnChooseServer->setMainText (tr ("Auto select"));
  ui->btnChooseServer->setSubText (tr ("choosing server"));

  ui->btnChooseSerial->setSubText (tr ("serial key"));

  ui->btnConnect->setText (tr ("connect"));

  ui->lDontHave->setText (tr ("Don't have a serial key?"));
  ui->btnObtainNewKey->setText (tr ("Tap here to obtain one"));
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
//  Q_UNUSED (a_serial);
//    qDebug () << __PRETTY_FUNCTION__ << a_serial;
//  auto text = ui->btnChooseSerial->mainText().remove ("-");

//  if (text.count() == MAX_COUNT_CHAR)
//    emit textEditedAndFilledOut (ui->btnChooseSerial->mainText());
//  else if (text.isEmpty())
//    emit textEditedAndCleaned();
//  else
//    emit sigSerialFillingIncorrect();
}

bool Login::_cbKeyEvent (DapGuiLineEdit *e, QKeyEvent *event)
{
    // delete selection
    int cursorPos = e->cursorPosition();
    if (((event->key() == Qt::Key_Delete) || (event->key() == Qt::Key_Backspace)) && e->hasSelectedText())
    {
        QString t = e->text();
        t = t.remove(e->selectionStart(), e->selectionLength()).remove("-");
        formatSerialKeyLine(t);
        e->clear();
        e->insert(t);
        e->setCursorPosition(e->selectionStart());
        return true;
    }
    // press del, delete one symbol
    if (event->key() == Qt::Key_Delete)
    {
        QString t = e->text();
        t.remove(cursorPos, 1).remove("-");
        formatSerialKeyLine(t);
        e->clear();
        e->insert(t);
        e->setCursorPosition(cursorPos);
        return true;
    }
    // press Backspace, delete one symbol
    if (event->key() == Qt::Key_Backspace)
    {
        QString t = e->text();
        if (cursorPos > 0)
        {
            int removeSym = 1;
            if ((t[t.length()-1] == "-") && (cursorPos==t.length()))
                removeSym = 2;
            t.remove(cursorPos-removeSym, removeSym).remove("-");
            formatSerialKeyLine(t);
            e->clear();
            e->insert(t);
            e->setCursorPosition(cursorPos);
            return true;
        }
    }
    // press Right or Left
    if ((event->key() == Qt::Key_Right) || (event->key() == Qt::Key_Left) ||
            (event->key() == Qt::Key_Home) || (event->key() == Qt::Key_End) ||
            event->matches(QKeySequence::StandardKey::Paste) || event->matches(QKeySequence::StandardKey::Copy) ||
            event->matches(QKeySequence::StandardKey::Undo) || event->matches(QKeySequence::StandardKey::Redo))
    {
        return false;
    }
    // symbol filtr
    QString charFiltr = "1234567890QWERTYUIOPASDFGHJKLZXCVBNM";
    foreach (auto ch, charFiltr)
    {
        if (QString(ch)==event->text().toUpper())
        {
            int insertPos = cursorPos;
            QString t = e->text().remove("-").toUpper();
            if (cursorPos > 4) insertPos -= 1;
            if (cursorPos > 9) insertPos -= 1;
            if (cursorPos > 14) insertPos -= 1;
            t.insert(insertPos, event->text().toUpper());
            formatSerialKeyLine(t);
            e->clear();
            e->insert(t);
            if (cursorPos == 3) cursorPos = 5;
            if (cursorPos == 8) cursorPos = 10;
            if (cursorPos == 13) cursorPos = 15;
            e->setCursorPosition(cursorPos+1);
            return true;
        }
    }
    return true;
}

/*-----------------------------------------*/
