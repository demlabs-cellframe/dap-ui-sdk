#ifndef DAPGUILABEL_H
#define DAPGUILABEL_H

/* INCLUDES */
#include <QLabel>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief overlap for label style
 *
 * Features:
 * - Css Style
 * - Mouse Button Press signal
 *
 * @ingroup groupDapGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiLabel : public QLabel
{
  Q_OBJECT
  DAPGUI_ENABLECSS
  bool _cssHook;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiLabel (QWidget *parent = nullptr);
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

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotCssStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUILABEL_H
