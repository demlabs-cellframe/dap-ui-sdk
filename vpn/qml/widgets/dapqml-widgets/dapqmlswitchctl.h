#ifndef DAPQMLSWITCHCTL_H
#define DAPQMLSWITCHCTL_H

/* INCLUDES */
#include <QObject>
#include <QScopedPointer>

/****************************************//**
 * @brief switch behavior controler class
 * @ingroup groupDapGuiStyle
 * @date 16.06.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlSwitchCtl : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (bool dragging               READ dragging               WRITE setDragging               NOTIFY sigDraggingChanged)
  Q_PROPERTY (bool draggingAnim           READ draggingAnim           WRITE setDraggingAnim           NOTIFY sigDraggingAnimChanged)
  Q_PROPERTY (bool draggingState          READ draggingState          WRITE setDraggingState          NOTIFY sigDraggingStateChanged)
  Q_PROPERTY (qreal pos1                  READ pos1                   WRITE setPos1                   NOTIFY sigPos1Changed)
  Q_PROPERTY (qreal pos2                  READ pos2                   WRITE setPos2                   NOTIFY sigPos2Changed)
  Q_PROPERTY (qreal diff                  READ diff                   WRITE setDiff                   NOTIFY sigDiffChanged)
  Q_PROPERTY (qreal draggingStartDistance READ draggingStartDistance  WRITE setDraggingStartDistance  NOTIFY sigDraggingStartDistanceChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  struct DapQmlSwitchCtlData;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  DapQmlSwitchCtlData *_data;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlSwitchCtl();
  ~DapQmlSwitchCtl();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE void setRoot (QObject *a_value);
  Q_INVOKABLE void setBackground (QObject *a_value);
  Q_INVOKABLE void setToggle (QObject *a_value);
  Q_INVOKABLE void setToggleAnimation (QObject *a_value);
  Q_INVOKABLE void setTouchingPoint (QObject *a_value);
  Q_INVOKABLE void setTouchArea (QObject *a_value);
  Q_INVOKABLE void updateTogglePos();

  bool dragging() const;
  void setDragging (bool a_value);

  bool draggingAnim() const;
  void setDraggingAnim (bool a_value);

  bool draggingState() const;
  void setDraggingState (bool a_value);

  qreal pos1() const;
  void setPos1 (qreal a_value);

  qreal pos2() const;
  void setPos2 (qreal a_value);

  qreal diff() const;
  void setDiff (qreal a_value);

  qreal draggingStartDistance() const;
  void setDraggingStartDistance (qreal a_value);

protected:
  bool isReady();

  void _begin();
  void _end();
  void _move();
  void _turnOff();
  void _turnOn();
  void _toggle();
  void _updateDiff();
  void _updateTglState();

  void _itemsCleared();

  void _print (const char *a_text);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigDraggingChanged();
  void sigDraggingAnimChanged();
  void sigDraggingStateChanged();
  void sigPos1Changed();
  void sigPos2Changed();
  void sigDiffChanged();
  void sigDraggingStartDistanceChanged();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotTouchingPointXChanged();
  void _slotTouchAreaPressed();
  void _slotTouchAreaReleased();
  void _slotUpdateTogglePos();
  void _slotStackUpdateTogglePos();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSWITCHCTL_H
