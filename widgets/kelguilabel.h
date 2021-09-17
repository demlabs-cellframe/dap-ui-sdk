#ifndef KELGUILABEL_H
#define KELGUILABEL_H

/* INCLUDES */
#include <QLabel>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief widget with custom properties
 * @ingroup groupKelGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiLabel : public QLabel
{
  Q_OBJECT
  KELGUI_ENABLECSS

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiLabel (QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void mousePressEvent (QMouseEvent *) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void clicked();         ///< button is clicked
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUILABEL_H
