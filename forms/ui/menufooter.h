#ifndef MENUFOOTER_H
#define MENUFOOTER_H

/* INCLUDES */
#include <QWidget>
#include <QMap>
#include <QPropertyAnimation>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class MenuFooter;}
QT_END_NAMESPACE

/****************************************//**
 * @brief menu footer widget
 *
 * Usualy lays on top of all screen widgets
 * at the bottom of the screen space
 *
 * @ingroup groupUiClasses
 * @date 27.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class MenuFooter : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum ButtonState
  {
    BS_NONE,
    BS_SETTINGS,
    BS_CONNECTION,
    BS_ACCAUNT,
  };
  Q_ENUM (ButtonState)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::MenuFooter *ui;
  ButtonState m_state;
  QMap<QObject *, ButtonState> m_statesMap;
  bool m_lock;                      ///< prevent recursion in slotButtonToggled
  bool m_active;                    ///< show/hide
  QPropertyAnimation *m_posAnim;    ///< animation
  struct
  {
    float y, height, screenHeight;
  } position;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit MenuFooter (QWidget *parent = nullptr);
  ~MenuFooter();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  bool active() const;
  void setActive (bool newActive);
  MenuFooter::ButtonState state() const;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigStateChanged (MenuFooter::ButtonState state);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  /// set progressbar value
  void slotSetProgressValue (int value);
  /// set progressbar range
  void slotSetProgressRange (int minimum, int maximum);
  /// change buttons states
  void slotSetButtonState (MenuFooter::ButtonState state);
  /// fix position
  void slotMoveToBottom();
  /// resize event
  void slotResizeEvent (QSize a_oldSize, QSize a_newSize);
private slots:
  /// react on user interaction
  void slotButtonToggled (bool checked);

  void _setAnimByState();
  void _startAnim();
  /// @}
};

/*-----------------------------------------*/
#endif // MENUFOOTER_H
