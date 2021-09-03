#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

/* INCLUDES */
#include <QWidget>

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
  Ui::SplashScreen *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit SplashScreen(QWidget *parent = nullptr);
  ~SplashScreen();
  /// @}
};

/*-----------------------------------------*/
#endif // SPLASHSCREEN_H
