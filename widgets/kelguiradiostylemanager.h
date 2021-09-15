#ifndef KELGUIRADIOSTYLEMANAGER_H
#define KELGUIRADIOSTYLEMANAGER_H

/* INCLUDES */
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief radio style overlap
 * @warning do not use this class outside
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiRadioStyleManager : public KelGuiStyleManager
{
  Q_OBJECT

  friend class KelGuiRadio;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  KelGuiRadioStyleManager (QWidget *parent);
  KelGuiRadioStyleManager (KelGuiRadioStyleManager &&src);
  ~KelGuiRadioStyleManager() override;
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
#endif // KELGUIRADIOSTYLEMANAGER_H
