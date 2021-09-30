#ifndef KELGUISWITCH_H
#define KELGUISWITCH_H

/* INCLUDES */
#include <QCheckBox>
#include "kelguiswitchstylemanager.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

/* DEFS */
namespace Ui { class KelGuiSwitch; };

/****************************************//**
 * @brief custom switch button
 * @ingroup groupKelGuiWidgets
 * @date 29.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiSwitch : public QWidget
{
  Q_OBJECT
  KELGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  /// form
  Ui::KelGuiSwitch *ui;
  /// styler
  KelGuiSwitchStyleManager __kgsm = KelGuiSwitchStyleManager (this);
  /// state
  bool m_checked;
  /// toggle & bg position x coordinate
  double m_toggleOnPos, m_bgPos[2];
  /// toogle animation group
  QParallelAnimationGroup *m_animGroup;
  QPropertyAnimation *m_animToggle;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiSwitch (QWidget *parent = 0);
  ~KelGuiSwitch();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  bool isChecked() const;
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
#endif // KELGUICHECKBOX_H
