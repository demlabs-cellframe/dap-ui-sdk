#ifndef KELGUISWITCHSTYLEMANAGER_H
#define KELGUISWITCHSTYLEMANAGER_H

/* INCLUDES */
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief radio style overlap
 * @warning do not use this class outside
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiSwitchStyleManager : public KelGuiStyleManager
{
  Q_OBJECT

  friend class KelGuiSwitch;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  KelGuiSwitchStyleManager (QWidget *parent);
  KelGuiSwitchStyleManager (KelGuiSwitchStyleManager &&src);
  ~KelGuiSwitchStyleManager() override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void styleUpdated();
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUISWITCHSTYLEMANAGER_H
