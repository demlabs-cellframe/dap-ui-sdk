/* INCLUDES */

#include "dapqmlarcanimation.h"
#include "dapqmlstyle.h"

#include <cmath>
#include <QBitmap>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlArcAnimation::DapQmlArcAnimation (QQuickItem *a_parent)
  : QQuickPaintedItem (a_parent)
  , _style (new DapQmlStyle)
  , m_strokeWidth (5)
  , m_startAngle (0)
{
  /* setup styler */

  _style->setItem (this);

  /* signals */

  connect (&m_animationTimer, &QTimer::timeout,
           this, &DapQmlArcAnimation::_slotUpdateAngle);
  connect (_style.get(), &DapQmlStyle::qssChanged,
           this, [this] { update(); });

  /* start animation */

  m_animationTimer.start (16);
}

DapQmlArcAnimation::~DapQmlArcAnimation()
{

}

/********************************************
 * METHODS
 *******************************************/

void DapQmlArcAnimation::init()
{
  qmlRegisterType<DapQmlArcAnimation> ("com.DapQmlWidgets", 1, 0, "DapQmlArcAnimation");
}

QString DapQmlArcAnimation::qss() const
{
  return _style->getQss();
}

void DapQmlArcAnimation::setQss (const QString &a_value)
{
  _style->setQss (a_value);
}

const QColor &DapQmlArcAnimation::color() const
{
  return m_color;
}

void DapQmlArcAnimation::setColor (const QColor &a_value)
{
  if (m_color == a_value)
    return;

  m_color = a_value;
  emit sigColorChanged();
  update();
}

int DapQmlArcAnimation::strokeWidth() const
{
  return m_strokeWidth;
}

void DapQmlArcAnimation::setStrokeWidth (int a_value)
{
  if (m_strokeWidth == a_value)
    return;

  m_strokeWidth = a_value;
  emit sigStrokeWidthChanged();
  update();
}

void DapQmlArcAnimation::paint (QPainter *a_painter)
{
  /* set smooth and transparent */
  QPen pen;
  QBrush brush (Qt::transparent);

  /* calc coordinates */
  qreal strokeWW   = qreal (m_strokeWidth) / 2.0;
  qreal centerX = width()  / 2.0;
  qreal centerY = height() / 2.0;
  qreal radiusX = width()  / 2.0 - m_strokeWidth;
  qreal radiusY = height() / 2.0 - m_strokeWidth;

  qreal startAngle = m_startAngle;
  qreal sweepAngle = 360;

  /* setup painter */
  a_painter->setBrush (brush);
  a_painter->setRenderHint (QPainter::Antialiasing);

  /* setup mask */
  QImage mask (width(), height(), QImage::Format_ARGB32);
  mask.fill (Qt::transparent);

  /* draw mask */
  QPainter maskPainter (&mask);
  maskPainter.translate (width() / 2, height() / 2);
  maskPainter.rotate (-m_startAngle);
  maskPainter.setBrush (Qt::white);
  maskPainter.drawRect (-width() / 2, 0, width(), height());
  maskPainter.end();

  /* lambda's */

  auto drawArc = [&]
  {
    /* setup painter */
    pen  = QPen (m_color, m_strokeWidth);
    a_painter->setPen (pen);

    /* draw arc */
    a_painter->drawArc(
      centerX - radiusX,
      centerY - radiusY,
      2 * radiusX, 2 * radiusY,
      int (startAngle * 16),
      int (sweepAngle * 16)
    );

  };

  auto drawDots = [&]
  {
    /* setup painter */
    pen  = QPen (m_color, strokeWW);
    a_painter->setPen (pen);

    /* draw dots on the ends of the arc */
    auto dots = _calcDotsPosition (radiusX, radiusY, strokeWW);

    a_painter->drawRoundedRect(
      dots.first,
      strokeWW,
      strokeWW
    );

    a_painter->drawRoundedRect(
      dots.second,
      strokeWW,
      strokeWW
    );
  };

  /* install mask */
  a_painter->setClipRegion (QRegion (QBitmap::fromImage (mask)));

  /* draw */

  drawArc();
  a_painter->setClipping (false);
  drawDots();
  //a_painter->drawImage (0, 0, mask);
}

QPair<QRectF, QRectF> DapQmlArcAnimation::_calcDotsPosition (qreal a_radiusX, qreal a_radiusY, qreal a_width) const
{
  qreal halfStrokeWidth = m_strokeWidth / 2.0;

  qreal angle   = (-m_startAngle) / 180.0 * M_PI;
  qreal centerX = width()  / 2.0 - halfStrokeWidth / 2.0;
  qreal centerY = height() / 2.0 - halfStrokeWidth / 2.0;

  return QPair<QRectF, QRectF>
  {
    {
      centerX + a_radiusX * std::cos (angle),
      centerY + a_radiusY * std::sin (angle),
      a_width,
      a_width
    },
    {
      centerX + a_radiusX * std::cos (angle + M_PI),
      centerY + a_radiusY * std::sin (angle + M_PI),
      a_width,
      a_width
    }
  };
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlArcAnimation::_slotUpdateAngle()
{
  m_startAngle += 2;
  if (m_startAngle >= 360)
      m_startAngle = 0;
  update();
}

/*-----------------------------------------*/
