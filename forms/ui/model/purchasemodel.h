#ifndef PURCHASEMODEL_H
#define PURCHASEMODEL_H

/* INCLUDES */
#include <QScrollArea>
#include <QBoxLayout>
#include "dapguibutton.h"

/****************************************//**
 * @brief purchase model list widget
 * @ingroup groupUiModels
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class PurchaseModel : public QScrollArea
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QVBoxLayout *lay;
  QList<DapGuiButton *> buttons;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit PurchaseModel (QWidget *parent = nullptr);
  ~PurchaseModel() override;
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup (QWidget *a_form);
  /// @}

  /****************************************//**
   * @name PRIVATE METHODS
   *******************************************/
  /// @{
private:
  void setupLayout();
  /// @}

};

/*-----------------------------------------*/
#endif // PURCHASEMODEL_H
