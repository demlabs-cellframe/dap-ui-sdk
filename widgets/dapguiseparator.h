#ifndef DAPGUISEPARATOR_H
#define DAPGUISEPARATOR_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
namespace Ui { class DapGuiSeparatorUI; };

/****************************************//**
 * @brief overlap for line style
 * @ingroup groupDapGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiSeparator : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::DapGuiSeparatorUI *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiSeparator(QWidget *parent = 0);
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
#endif // DAPGUISEPARATOR_H
