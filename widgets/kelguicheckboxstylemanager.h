#ifndef KELGUICHECKBOXSTYLEMANAGER_H
#define KELGUICHECKBOXSTYLEMANAGER_H

/* INCLUDES */
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief check box style overlap
 * @warning do not use this class outside
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiCheckBoxStyleManager : public KelGuiStyleManager
{
  Q_OBJECT

  friend class KelGuiCheckBox;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  KelGuiCheckBoxStyleManager (QWidget *parent);
  KelGuiCheckBoxStyleManager (KelGuiCheckBoxStyleManager &&src);
  ~KelGuiCheckBoxStyleManager() override;
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected slots:
  void forcedCheckboxStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUICHECKBOXSTYLEMANAGER_H
