#ifndef PURCHASE_H
#define PURCHASE_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Purchase;}
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for licence purchases
 *
 * Purchase licence right inside app using
 * this form page
 *
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class Purchase : public BaseForm
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
  explicit Purchase (QWidget *parent = nullptr);
  ~Purchase();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  /// @}
};

/*-----------------------------------------*/
#endif // PURCHASE_H
