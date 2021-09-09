#ifndef STATISTICS_H
#define STATISTICS_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Statistics; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for statistics display
 * @ingroup groupUiClasses
 * @date 27.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class Statistics : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Statistics *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Statistics (QWidget *parent = nullptr);
  ~Statistics() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void restartUi() override;
  /// @}
};

/*-----------------------------------------*/
#endif // STATISTICS_H
