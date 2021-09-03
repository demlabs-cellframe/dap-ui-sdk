#ifndef LOGIN_H
#define LOGIN_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

/****************************************//**
 * @date 25.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class Login : public QWidget
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
  explicit Login(QWidget *parent = nullptr);
  ~Login();
  /// @}
};

/*-----------------------------------------*/
#endif // LOGIN_H
