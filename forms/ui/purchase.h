#ifndef PURCHASE_H
#define PURCHASE_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Purchase;}
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for licence purchases
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class Purchase : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Purchase *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Purchase(QWidget *parent = nullptr);
  ~Purchase();
  /// @}
};

/*-----------------------------------------*/
#endif // PURCHASE_H
