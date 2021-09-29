#ifndef KELGUISWITCH_H
#define KELGUISWITCH_H

/* INCLUDES */
#include <QCheckBox>
#include "kelguiswitchstylemanager.h"

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

  Ui::KelGuiSwitch *ui;
  KelGuiSwitchStyleManager __kgsm = KelGuiSwitchStyleManager (this);
  bool m_checked;

  void _debugInfoClipboard();

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
};

/*-----------------------------------------*/
#endif // KELGUICHECKBOX_H
