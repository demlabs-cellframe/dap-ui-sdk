#ifndef KELGUISEPARATOR_H
#define KELGUISEPARATOR_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
namespace Ui { class KelGuiSeparatorUI; };

/****************************************//**
 * @brief overlap for line style
 * @ingroup groupKelGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiSeparator : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::KelGuiSeparatorUI *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiSeparator(QWidget *parent = 0);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void paintEvent (QPaintEvent *) override;
  /// @}

};

/*-----------------------------------------*/
#endif // KELGUISEPARATOR_H
