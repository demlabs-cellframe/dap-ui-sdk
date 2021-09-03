#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

/* INCLUDES */
#include "modelbase.h"

/****************************************//**
 * @brief settings model list widget
 * @date 31.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class SettingsModel : public ModelBase
{
  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit SettingsModel (QWidget *parent = nullptr);
  ~SettingsModel() override;
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  /// @}
};

/*-----------------------------------------*/
#endif // SETTINGSMODEL_H
