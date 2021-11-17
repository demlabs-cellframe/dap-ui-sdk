#ifndef DAPGUIPLAINTEXTEDITSTYLEMANAGER_H
#define DAPGUIPLAINTEXTEDITSTYLEMANAGER_H

/* INCLUDES */
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief plain text edit style overlap
 *
 * @warning do not use this class outside
 * DapGuiPlainTextEdit
 * @date 18.11.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiPlainTextEditStyleManager : public DapGuiStyleManager
{
  Q_OBJECT

  friend class DapGuiPlainTextEdit;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapGuiPlainTextEditStyleManager (QWidget *parent);
  DapGuiPlainTextEditStyleManager (DapGuiPlainTextEditStyleManager &&src);
  ~DapGuiPlainTextEditStyleManager() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
protected:
  void updatePlaceholderStyle (bool a_isEmpty);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void forcedPteStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIPLAINTEXTEDITSTYLEMANAGER_H
