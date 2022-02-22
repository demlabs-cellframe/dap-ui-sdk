#ifndef LOGIN_H
#define LOGIN_H

/* INCLUDES */
#include <QWidget>
#include "dapguiplaintextedit.h"
#include "dapguibutton.h"
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

#ifndef MAX_COUNT_CHAR
#define MAX_COUNT_CHAR 16
#endif

/****************************************//**
 * @brief ui/class for login page
 *
 * Login screen
 *
 * Features:
 * - Insert serial key
 * - Choose server to connect
 * - Open "buy key" webpage
 *
 * @ingroup groupUiClasses
 * @date 25.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class Login : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum ErrorColor
  {
    ERRCOL_BLACK,
    ERRCOL_RED,
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Login *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Login (QWidget *parent = nullptr);
  ~Login();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString getSerialNumber();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigChooseServer();
  void sigChooseSerial();
  void sigConnect();
  void sigObtainNewKey();

  void textEditedAndCleaned();
  void textEditedAndFilledOut(QString);

  void textChangedAndCleaned();
  void textChangedAndFilledOut(QString);

  void sigSerialFillingIncorrect();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  /// show/hide serial key input
  void slotKeyEnable (bool enable);
  /// enable/disable "connect" button
  void slotSetConnectBtnEnabled (bool enable);
  /// change button text
  void slotSetConnectBtnText(QString a_text);
  /// show error text
  void slotErrorText (QString text, ErrorColor color = ErrorColor::ERRCOL_RED);
  /// change server name text
  void slotChangeServerName (QString serverName);
  /// change inserted serial
  void slotChangeSerial (QString serialKey);
  /// set server choose button active
  void slotServerChooserActivate();
  /// translate contents
  void slotRetranslated();
  void setServerInfo(QString a_name, QString a_ip);
private slots:
  void _slotSerialChanged (const QString &a_serial);
  void _slotSerialEdited (const QString &a_serial);
protected:
  static bool _cbKeyEvent (DapGuiLineEdit *e, QKeyEvent *event);
  /// @}
};

/*-----------------------------------------*/
#endif // LOGIN_H
