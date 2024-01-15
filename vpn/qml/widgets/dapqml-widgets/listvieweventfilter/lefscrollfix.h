#ifndef LEFSCROLLFIX_H
#define LEFSCROLLFIX_H

/* INCLUDES */

#include "lefbase.h"

/****************************************//**
 * @brief lef scroll fix class
 *
 * Basicaly replaces angleDelta with pixelDelta
 * and pixelDelta with nothing
 *
 * @date 05.12.2023
 * @author Mikhail Shilenko
 *******************************************/

class LefScrollFix : public LefBase
{
  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit LefScrollFix();
  ~LefScrollFix() override;
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
#endif // LEFSCROLLFIX_H
