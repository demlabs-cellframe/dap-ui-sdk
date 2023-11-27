#ifndef DAPQMLLISTVIEWEVENTFILTER_H
#define DAPQMLLISTVIEWEVENTFILTER_H

/* INCLUDES */

#include <QQuickItem>

/****************************************//**
 * @brief qml listview event filter
 * @ingroup groupInterface
 * @date 13.11.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlListviewEventFilter : public QQuickItem
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  struct Data;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  Data *data;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlListviewEventFilter (QQuickItem *a_parent = nullptr);
  ~DapQmlListviewEventFilter() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static void attachFilter (QObject *a_object);
  bool eventFilter (QObject *a_watched, QEvent *a_event) override;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotFakeEvent();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLLISTVIEWEVENTFILTER_H
