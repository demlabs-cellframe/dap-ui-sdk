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
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef double (*P2pCallback) (double points);
  /// @}

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
  /// toggle position x coordinate
  double m_toggleOnPos;
  /// toogle animation group
  QParallelAnimationGroup m_animGroup;
  QPropertyAnimation *m_animToggle;

  /// callback for UiScaling::pointsToPixels
  static P2pCallback m_p2p;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiSwitch (QWidget *parent = 0);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  bool isChecked() const;
  void setChecked (bool a_checked);
  void toggle();
  /// set UiScaling::pointsToPixels function pointer
  void setP2p(P2pCallback newP2p);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void stateChanged (bool);
  void clicked();
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
  void _debugInfoClipboard();
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUICHECKBOX_H
