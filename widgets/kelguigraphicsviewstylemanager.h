#ifndef KELGUIGRAPHICSVIEWSTYLEMANAGER_H
#define KELGUIGRAPHICSVIEWSTYLEMANAGER_H

/* INCLUDES */
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief graphics view style overlap
 * @warning do not use this class outside
 * @date 16.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiGraphicsViewStyleManager : public KelGuiStyleManager
{
  friend class KelGuiGraphicsView;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  KelGuiGraphicsViewStyleManager (QWidget *parent);
  KelGuiGraphicsViewStyleManager (KelGuiGraphicsViewStyleManager &&src);
  ~KelGuiGraphicsViewStyleManager() override;
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected slots:
  void forcedGraphicsViewStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIGRAPHICSVIEWSTYLEMANAGER_H
