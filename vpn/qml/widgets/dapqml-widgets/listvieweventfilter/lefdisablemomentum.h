#ifndef LEFDISABLEMOMENTUM_H
#define LEFDISABLEMOMENTUM_H

/* INCLUDES */

#include "lefbase.h"

/****************************************//**
 * @brief lef disable momentum class
 *
 * Disable momentum events
 *
 * @date 05.12.2023
 * @author Mikhail Shilenko
 *******************************************/

class LefDisableMomentum : public LefBase
{
  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit LefDisableMomentum();
  ~LefDisableMomentum() override;
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
#endif // LEFDISABLEMOMENTUM_H
