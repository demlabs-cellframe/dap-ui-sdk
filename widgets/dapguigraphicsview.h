#ifndef DAPGUIGRAPHICSVIEW_H
#define DAPGUIGRAPHICSVIEW_H

/* INCLUDES */
#include <QGraphicsView>
#include "dapguigraphicsviewstylemanager.h"

/****************************************//**
 * @brief overlap for checkbox style
 * @ingroup groupDapGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiGraphicsView : public QGraphicsView
{
  Q_OBJECT

  DapGuiGraphicsViewStyleManager __kgsm = DapGuiGraphicsViewStyleManager (this);

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiGraphicsView (QWidget *parent = 0);
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
#endif // DAPGUIGRAPHICSVIEW_H
