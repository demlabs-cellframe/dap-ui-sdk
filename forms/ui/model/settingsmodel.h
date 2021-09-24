#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

/* INCLUDES */
#include "modelbase.h"
#include "style/kelguistylemanager.h"

class Settings;

/****************************************//**
 * @brief settings model list widget
 * @ingroup groupUiModels
 * @date 31.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class SettingsModel : public ModelBase
{
  KELGUI_ENABLECSS
  //KELGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit SettingsModel (QWidget *parent = nullptr);
  ~SettingsModel() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  void setInterface (Settings *s);
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  void slotSetDaysLeft (QString days);
  void slotClicked();
  /// @}
};

/*-----------------------------------------*/
#endif // SETTINGSMODEL_H
