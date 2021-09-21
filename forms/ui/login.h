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
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotKeyEnable (bool enable);
  void slotErrorText (QString text);
  void slotChangeServerName (QString serverName);
  void slotChangeSerial (QString serialKey);
  void slotServerChooserActivate();
  void slotClearStatusLable();
  void setSerialNumber(const QString &a_serial);
  /// @}
};

/*-----------------------------------------*/
#endif // LOGIN_H
