#ifndef DAPGUICHECKBOXSTYLEMANAGER_H
#define DAPGUICHECKBOXSTYLEMANAGER_H

/* INCLUDES */
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief check box style overlap
 * @warning do not use this class outside
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiCheckBoxStyleManager : public DapGuiStyleManager
{
  Q_OBJECT

  friend class DapGuiCheckBox;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapGuiCheckBoxStyleManager (QWidget *parent);
  DapGuiCheckBoxStyleManager (DapGuiCheckBoxStyleManager &&src);
  ~DapGuiCheckBoxStyleManager() override;
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
#endif // DAPGUICHECKBOXSTYLEMANAGER_H
