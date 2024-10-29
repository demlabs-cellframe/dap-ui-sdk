#ifndef DAPGUIRADIOSTYLEMANAGER_H
#define DAPGUIRADIOSTYLEMANAGER_H

/* INCLUDES */
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief radio style overlap
 * @warning do not use this class outside
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiRadioStyleManager : public DapGuiStyleManager
{
  Q_OBJECT

  friend class DapGuiRadio;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapGuiRadioStyleManager (QWidget *parent);
  DapGuiRadioStyleManager (DapGuiRadioStyleManager &&src);
  ~DapGuiRadioStyleManager() override;
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected slots:
  void forcedRadioStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIRADIOSTYLEMANAGER_H
