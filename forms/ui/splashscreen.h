#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

/* INCLUDES */
#include <QWidget>
#include "vpnDefine.h"

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

class SplashScreen : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::SplashScreen *m_ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:

    struct StateLinks;

    SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();
  /// @}

  void setState(ConnectionState a_state);
  void setErrorMessage(const QString &msg);
};

/*-----------------------------------------*/
#endif // SPLASHSCREEN_H
