#ifndef KELGUIGRAPHICSVIEW_H
#define KELGUIGRAPHICSVIEW_H

/* INCLUDES */
#include <QGraphicsView>
#include "kelguigraphicsviewstylemanager.h"

/****************************************//**
 * @brief overlap for checkbox style
 * @ingroup groupKelGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiGraphicsView : public QGraphicsView
{
  Q_OBJECT

  KelGuiGraphicsViewStyleManager __kgsm = KelGuiGraphicsViewStyleManager (this);

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiGraphicsView (QWidget *parent = 0);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void resizeEvent (QResizeEvent *event) override;
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIGRAPHICSVIEW_H
