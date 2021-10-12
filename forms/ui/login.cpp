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
static int s_oldSerialLength  = 0;
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
  ui->btnChooseSerial->setBtnStyle (KelGuiButton::TopMainBottomSub);
  ui->btnChooseSerial->setMainText ("____-____-____-____");
#endif // Q_OS_ANDROID

//  ui->btnChooseSerial->setInputFocusCallback (cbSerialFocus);
//#ifdef Q_OS_ANDROID
//  ui->btnChooseSerial->setInputCallback (cbSerialText);
//#else
//  ui->btnChooseSerial->setInputMask (">NNNN-NNNN-NNNN-NNNN;_");
//#endif // Q_OS_ANDROID

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

void Login::cbSerialFocus (KelGuiLineEdit *e, const Qt::FocusReason &reason)
{
  Q_UNUSED (e)

  if (reason == Qt::MouseFocusReason)
    {
      auto btn  = __inst->ui->btnChooseSerial;
      btn->setMainText ("");
      s_enteredSerial.clear();
    }
}

void Login::cbSerialText (KelGuiLineEdit *e, QString &preedit, QString &commit, int from, int to)
{
  /* compare length's */
  //s_oldSerialLength = e->text().length();
  int len     = (preedit.length() > commit.length())
                ? preedit.length()
                : commit.length();
  auto &text  = (preedit.length() > commit.length())
                ? preedit
                : commit;
//  if (from || to)
//    {
//      s_enteredSerial.replace (e->selectionStart() + from, to, commit);
//    }

  /* fix strings */
  fixSerialString (e, preedit, true);
  fixSerialString (e, commit, true);

  /* loose focus */
  if (len == EXPECT_LENGTH && s_oldSerialLength == EXPECT_LENGTH)
    {
      __inst->ui->btnConnect->setFocus (Qt::MouseFocusReason);
      __inst->ui->btnChooseSerial->setMainText (text);
      qDebug() << "finished with text:" << text;
    }

  qDebug() << __PRETTY_FUNCTION__
           << "old" << s_oldSerialLength
           << "len" << len
           << "commit" << commit
           << "preedit" << preedit
           << "text" << e->text();

  /* update old */
  s_oldSerialLength = len;
}

void Login::fixSerialString (KelGuiLineEdit *e, QString &serial, bool inserted)
{
  /* get copy */
  auto text   = serial.toUpper();

  /* fix letters */
  for (auto i = text.begin(), e = text.end(); i != e; i++)
    if ((*i < 0x30)
        || ((*i > 0x39) && (*i < 0x41))
        || (*i > 0x5A))
      *i = 'A';

  if (inserted)
    {
      /* add separator */
      if (text.length() == 4
          || text.length() == 9
          || text.length() == 14)
        {
          text += '-';
          //e->cursorForward (false);
          //e->setCursorPosition (text.length());
        }

      /* insert separator */
      if (text.length() >= 4 && text.at (4) != '-')
        text.insert (4, '-');
      if (text.length() >= 9 && text.at (9) != '-')
        text.insert (9, '-');
      if (text.length() >= 14 && text.at (14) != '-')
        text.insert (14, '-');
    }

//  /* replace separators */
//  if (text.length() >= 4)
//    text.replace (4, 1, '-');
//  if (text.length() >= 9)
//    text.replace (9, 1, '-');
//  if (text.length() >= 14)
//    text.replace (14, 1, '-');

  /* check if limit reachced */
  if (text.length() < MAX_LENGTH)
    {
      if (!text.isEmpty())
        serial  = text;
      return;
    }
  else
    {
      /* make loose focus */
      QEvent event (QEvent::FocusOut);
      QApplication::sendEvent (e, &event);
    }

  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  serial    = text;
}

void Login::fixSerialString_ (KelGuiLineEdit *e, QString &serial, bool inserted)
{
  //qDebug() << __PRETTY_FUNCTION__ << "start";

  /* get input text */
  auto text   = serial.toUpper();

  /* add separator */
  if (text.length() == 3
      || text.length() == 8
      || text.length() == 13)
    {
      text += '-';
      e->cursorForward (false);
    }

  /* add separators */
  for (int i = 0; i < text.length(); i++)
    {
      /* AAAA-AAAA-AAAA-AAAA */
      /*     4    9    E     */

//      /* insert separators */
//      if ((i == 4) || (i == 9) || (i == 14))
//        {
//          //qDebug() << __PRETTY_FUNCTION__ << "sep";
//          if (i == 4)
//            if (text.at(4) != '-')
//              {
//                text.insert(4,'-');
//                e->cursorForward (false);
//              }
//          if (i == 9)
//            if (text.at(9) != '-')
//              {
//                text.insert(9,'-');
//                e->cursorForward (false);
//              }
//          if (i == 14)
//            if (text.at(14) != '-')
//              {
//                text.insert(14,'-');
//                e->cursorForward (false);
//              }
//        }

//      /* replace wrong characters */
//      else
      {
        //qDebug() << __PRETTY_FUNCTION__ << "wr";
        QChar c = text.at (i);
        if (c < 0x30

            || (c > 0x39
                && c < 0x41)

            || c > 0x5A

            || (
              c == '-'
              && (i != 4) && (i != 9) && (i != 14)
            )
           )
          {
            text.remove (i--, 1);
            e->cursorBackward (false);
          }
      }
    }

  /* check if limit reachced */
  if (text.length() < MAX_LENGTH)
    {
      //qDebug() << __PRETTY_FUNCTION__ << "skippo";
      if (!text.isEmpty())
        serial  = text;

      //qDebug() << __PRETTY_FUNCTION__ << "skipped" << text;
      return;
    }
  else
    {
      /* make loose focus */
      QEvent event (QEvent::FocusOut);
      QApplication::sendEvent (e, &event);
    }

  //qDebug() << __PRETTY_FUNCTION__ << "finno";
  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  serial    = text;

  qDebug() << __PRETTY_FUNCTION__ << "result" << text;
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

void Login::slotClearStatusLable()
{
  ui->lStatus->clear();
}

void Login::setSerialNumber (const QString &a_serial)
{
  ui->btnChooseSerial->setMainText (a_serial);
}

void Login::slotSerialChanged (const QString &a_serial)
{
  Q_UNUSED (a_serial);

  auto text = ui->btnChooseSerial->mainText().remove ("-");

  if (text.count() == MAX_COUNT_CHAR)
    emit textChangedAndFilledOut (ui->btnChooseSerial->mainText());
  else if (text.isEmpty())
    emit textChangedAndCleaned();
  else
    emit slotSerialFillingIncorrect();
}

void Login::slotSerialEdited (const QString &a_serial)
{
  Q_UNUSED (a_serial);

  auto text = ui->btnChooseSerial->mainText().remove ("-");

  if (text.count() == MAX_COUNT_CHAR)
    emit textEditedAndFilledOut (ui->btnChooseSerial->mainText());
  else if (text.isEmpty())
    emit textEditedAndCleaned();
  else
    emit slotSerialFillingIncorrect();
}

/*-----------------------------------------*/
