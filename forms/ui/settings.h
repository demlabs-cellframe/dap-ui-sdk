#ifndef SETTINGS_H
#define SETTINGS_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE

/****************************************//**
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class Settings : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Settings *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Settings(QWidget *parent = nullptr);
  ~Settings();
  /// @}
};

/*-----------------------------------------*/
#endif // SETTINGS_H
