#ifndef LOGIN_H
#define LOGIN_H

/* INCLUDES */
#include <QWidget>
#include <QStateMachine>
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

  void slotSerialFillingIncorrect();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotKeyEnable (bool enable);
  void slotSetConnectBtnEnabled (bool enable);
  void slotSetConnectBtnText(QString a_text);
  void slotErrorText (QString text, ErrorColor color = ErrorColor::ERRCOL_RED);
  void slotChangeServerName (QString serverName);
  void slotChangeSerial (QString serialKey);
  void slotServerChooserActivate();
  void slotClearStatusLable();
  void setSerialNumber (const QString &a_serial);
  /// @}
};

/*-----------------------------------------*/
#endif // LOGIN_H
