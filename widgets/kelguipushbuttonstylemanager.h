#ifndef KELGUIPUSHBUTTONSTYLEMANAGER_H
#define KELGUIPUSHBUTTONSTYLEMANAGER_H

/* INCLUDES */
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief push button style overlap
 * @warning do not use this class outside
 * @date 13.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiPushButtonStyleManager : public KelGuiStyleManager
{
  Q_OBJECT

  friend class KelGuiPushButton;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  KelGuiPushButtonStyleManager (QWidget *parent);
  KelGuiPushButtonStyleManager (KelGuiPushButtonStyleManager &&src);
  ~KelGuiPushButtonStyleManager() override;
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void setCssStyle (const QString &cssStyle);
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected slots:
  void forcedButtonStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIPUSHBUTTONSTYLEMANAGER_H
