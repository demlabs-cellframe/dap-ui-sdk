#ifndef DAPGUIPUSHBUTTONSTYLEMANAGER_H
#define DAPGUIPUSHBUTTONSTYLEMANAGER_H

/* INCLUDES */
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief push button style overlap
 * @warning do not use this class outside
 * @date 13.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiPushButtonStyleManager : public DapGuiStyleManager
{
  Q_OBJECT

  friend class DapGuiPushButton;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapGuiPushButtonStyleManager (QWidget *parent);
  DapGuiPushButtonStyleManager (DapGuiPushButtonStyleManager &&src);
  ~DapGuiPushButtonStyleManager() override;
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
#endif // DAPGUIPUSHBUTTONSTYLEMANAGER_H
