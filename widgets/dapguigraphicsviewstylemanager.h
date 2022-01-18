#ifndef DAPGUIGRAPHICSVIEWSTYLEMANAGER_H
#define DAPGUIGRAPHICSVIEWSTYLEMANAGER_H

/* INCLUDES */
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief graphics view style overlap
 *
 * Connects to signal forceStyleUpdate
 * with forcedGraphicsViewStyleUpdate
 *
 * Used to change Scene size when style
 * updates requested (after style change)
 *
 * @warning do not use this class outside
 * DapGuiGraphicsView
 * @date 16.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiGraphicsViewStyleManager : public DapGuiStyleManager
{
  friend class DapGuiGraphicsView;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapGuiGraphicsViewStyleManager (QWidget *parent);
  DapGuiGraphicsViewStyleManager (DapGuiGraphicsViewStyleManager &&src);
  ~DapGuiGraphicsViewStyleManager() override;
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
public slots:
  void forcedGraphicsViewStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIGRAPHICSVIEWSTYLEMANAGER_H
