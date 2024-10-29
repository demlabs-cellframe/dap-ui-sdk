#ifndef DAPGUISWITCHSTYLEMANAGER_H
#define DAPGUISWITCHSTYLEMANAGER_H

/* INCLUDES */
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief radio style overlap
 * @warning do not use this class outside
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiSwitchStyleManager : public DapGuiStyleManager
{
  Q_OBJECT

  friend class DapGuiSwitch;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapGuiSwitchStyleManager (QWidget *parent);
  DapGuiSwitchStyleManager (DapGuiSwitchStyleManager &&src);
  ~DapGuiSwitchStyleManager() override;
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
#endif // DAPGUISWITCHSTYLEMANAGER_H
