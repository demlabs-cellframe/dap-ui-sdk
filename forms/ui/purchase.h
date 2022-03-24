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
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Product
  {
    Undefined,

    Key1month,
    Key6months,
    Key12months,
  };
  Q_ENUM(Product)
  /// @}

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

  void	showEvent(QShowEvent *event) override;

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  void sigPurchase (Product a_product);
  void showSignal();
  /// @}
};

/*-----------------------------------------*/
#endif // PURCHASE_H
