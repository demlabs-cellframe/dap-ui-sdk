#ifndef LEFCOLLECTOR_H
#define LEFCOLLECTOR_H

/* INCLUDES */

#include "lefbase.h"

/****************************************//**
 * @brief lef collector class
 *
 * Collects events and after timeout sends
 * mid value as an event.
 *
 * @date 05.12.2023
 * @author Mikhail Shilenko
 *******************************************/

class LefCollector : public LefBase
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
  explicit LefCollector();
  ~LefCollector() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void process (QObject *a_target, Data &a_data) override;
  /// @}
};

/*-----------------------------------------*/
#endif // LEFCOLLECTOR_H
