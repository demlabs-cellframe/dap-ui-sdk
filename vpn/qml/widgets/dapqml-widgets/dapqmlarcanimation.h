#ifndef DAPQMLARCANIMATION_H
#define DAPQMLARCANIMATION_H

/* INCLUDES */

#include <QQuickPaintedItem>
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QScopedPointer>

/* DEFS */

class DapQmlStyle;

/****************************************//**
 * @brief Arc Animation Widget
 * @ingroup groupDapGuiStyle
 * @date 14.12.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlArcAnimation : public QQuickPaintedItem
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString qss READ qss WRITE setQss NOTIFY sigQssChanged)
  Q_PROPERTY (QColor color READ color WRITE setColor NOTIFY sigColorChanged)
  Q_PROPERTY (int strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY sigStrokeWidthChanged)
  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
private:
  QScopedPointer<DapQmlStyle> _style;
  QColor m_color;
  int m_strokeWidth;
  QTimer m_animationTimer;
  qreal m_startAngle;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlArcAnimation (QQuickItem *a_parent = nullptr);
  ~DapQmlArcAnimation();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static void init();

  QString qss() const;
  void setQss (const QString &a_value);

  const QColor &color() const;
  void setColor (const QColor &a_value);

  int strokeWidth() const;
  void setStrokeWidth (int a_value);

protected:
  void paint (QPainter *a_painter) override;
  QPair<QRectF, QRectF> _calcDotsPosition (qreal a_radiusX, qreal a_radiusY, qreal a_width) const;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigQssChanged();
  void sigColorChanged();
  void sigStrokeWidthChanged();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
private slots:
  void _slotUpdateAngle();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLARCANIMATION_H
