#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

/* INCLUDES */
#include <QWidget>
#include "vpnDefine.h"
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class SplashScreen; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for splash into screen
 * @ingroup groupUiClasses
 * @date 25.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class SplashScreen : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
  struct StateLinks;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::SplashScreen *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit SplashScreen (QWidget *parent = nullptr);
  ~SplashScreen();
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  void setState (ConnectionState a_state);
  void setErrorMessage (const QString &msg);
  /// @}
};

/*-----------------------------------------*/
#endif // SPLASHSCREEN_H
