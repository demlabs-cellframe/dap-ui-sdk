#ifndef LEFBASE_H
#define LEFBASE_H

/* INCLUDES */

#include <QtCore>

/****************************************//**
 * @brief listview event filter base class
 * @date 30.11.2023
 * @author Mikhail Shilenko
 *******************************************/

class LefBase
{
  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  struct Data
  {
    QPointF position;
    QPointF globalPosition;
    QPoint pixelDelta;
    QPoint angleDelta;
    Qt::MouseButtons buttons;
    Qt::KeyboardModifiers modifiers;
    Qt::ScrollPhase phase;
    bool inverted;
    Qt::MouseEventSource source;
    bool eventIsHandled; // set this true - if you want to skip this event
  };
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit LefBase() {}
  virtual ~LefBase() {}
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  virtual void process (QObject *a_target, Data &a_data) {}
  /// @}
};

/*-----------------------------------------*/
#endif // LEFBASE_H
