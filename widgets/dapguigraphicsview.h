#ifndef DAPGUIGRAPHICSVIEW_H
#define DAPGUIGRAPHICSVIEW_H

/* INCLUDES */
#include <QGraphicsView>
#include "dapguigraphicsviewstylemanager.h"

/****************************************//**
 * @brief overlap for graphics view style
 *
 * Using inerhit from DapGuiStyleManager
 * class DapGuiGraphicsViewStyleManager to
 * update scene size at style update
 *
 * @ingroup groupDapGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiGraphicsView : public QGraphicsView
{
  Q_OBJECT

  DapGuiGraphicsViewStyleManager __kgsm = DapGuiGraphicsViewStyleManager (this);
  QObject *m_form; ///< Statistics form. required to apply colors

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiGraphicsView (QWidget *parent = 0);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QObject *form() const;
  void setForm (QObject *a_form);
  void updateG ();
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
