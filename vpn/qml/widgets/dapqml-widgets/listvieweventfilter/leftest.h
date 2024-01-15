#ifndef LEFTEST_H
#define LEFTEST_H

/* INCLUDES */

#include "lefbase.h"

/****************************************//**
 * @brief lef test class
 * @date 30.11.2023
 * @author Mikhail Shilenko
 *******************************************/

class LefTest : public LefBase
{
  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  struct Private;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  Private *data;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit LefTest();
  ~LefTest() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void process (QObject *a_target, Data &a_data) override;
  void pushTestEvent();
  /// @}
};

/*-----------------------------------------*/
#endif // LEFTEST_H
