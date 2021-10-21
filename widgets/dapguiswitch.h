#ifndef DAPGUISWITCH_H
#define DAPGUISWITCH_H

/* INCLUDES */
#include <QCheckBox>
#include "dapguiswitchstylemanager.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

/* DEFS */
namespace Ui { class DapGuiSwitch; };

/****************************************//**
 * @brief custom switch button
 *
 * Widget made using 2 DapGuiWidget 's
 * inside ui-form
 *
 * Features:
 * - Css Style
 * - Acting like checkbox
 * - Has animated moving part
 * - Automaticaly change size on need
 * - Changing color on different states
 *
 * @ingroup groupDapGuiWidgets
 * @date 29.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiSwitch : public QWidget
{
  Q_OBJECT
  DAPGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (bool checked  READ checked  WRITE setChecked)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  /// form
  Ui::DapGuiSwitch *ui;
  /// styler
  DapGuiSwitchStyleManager __kgsm = DapGuiSwitchStyleManager (this);
  /// state
  bool m_checked;
  /// toggle & bg position x coordinate
  double m_toggleOnPos, m_bgPos[2];
  /// toogle animation group
  QParallelAnimationGroup *m_animGroup;
  /// toggle animation
  QPropertyAnimation *m_animToggle;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapGuiSwitch (QWidget *parent = nullptr);
  ~DapGuiSwitch();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  bool checked() const;
  void setChecked (bool a_checked);
  void toggle();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void stateChanged (bool);
  void clicked();
  void toggled(bool);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void mousePressEvent (QMouseEvent *) override;
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected slots:
  void _setAnimByState();
  void _moveItems();
  void _debugInfoClipboard();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUICHECKBOX_H
