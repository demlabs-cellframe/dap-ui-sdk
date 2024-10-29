#ifndef DAPQMLSEPARATORITEM_H
#define DAPQMLSEPARATORITEM_H

/* INCLUDES */
#include <QQuickPaintedItem>

/****************************************//**
 * @brief QML Separator
 * @ingroup groupDapGuiStyle
 * @date 12.07.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlSeparatorItem : public QQuickPaintedItem
{
  Q_OBJECT
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QQuickItem *_root;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlSeparatorItem (QQuickItem *a_parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void setRootItem (QQuickItem *a_root);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void paint (QPainter *a_painter) override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSEPARATORITEM_H
