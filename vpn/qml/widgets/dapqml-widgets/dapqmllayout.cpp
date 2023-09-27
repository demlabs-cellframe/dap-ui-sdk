/* INCLUDES */
#include "dapqmllayout.h"
#include <QSGNode>
#include <QSGSimpleRectNode>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlLayout::DapQmlLayout (QQuickItem *a_parent)
  : QQuickItem (a_parent)
  , m_spacing (4.0)
  , m_contentWidth (0)
  , m_contentHeight (0)
{
  setFlag (ItemHasContents, true);
}

DapQmlLayout::~DapQmlLayout()
{

}

/********************************************
 * METHODS
 *******************************************/

qreal DapQmlLayout::spacing() const
{
  return m_spacing;
}

void DapQmlLayout::setSpacing (qreal a_value)
{
  if (m_spacing == a_value)
    return;

  m_spacing = a_value;
  emit sigSpacingChanged();
  update(); // register redraw event
}

qreal DapQmlLayout::contentWidth() const
{
  return m_contentWidth;
}

void DapQmlLayout::setContentWidth (qreal a_value)
{
  if (m_contentWidth == a_value)
    return;

  m_contentWidth = a_value;
  emit sigContentWidthChanged();
}

qreal DapQmlLayout::contentHeight() const
{
  return m_contentHeight;
}

void DapQmlLayout::setContentHeight (qreal a_value)
{
  if (m_contentHeight == a_value)
    return;

  m_contentHeight = a_value;
  emit sigContentHeightChanged();
}
/********************************************
 * OVERRIDE
 *******************************************/

QSGNode *DapQmlLayout::updatePaintNode (QSGNode *, UpdatePaintNodeData *)
{
  /* arrange items inside */
  QPointF pos { 0, 0 };
  QSizeF content { 0, 0 };
  QSizeF canvasSize = size();
  qreal maxWidth  = 0;
  qreal maxHeight = 0;
  QList<QQuickItem *> items = childItems();
  qDebug() << __func__ << boundingRect();
  int i = 0;
  for (QQuickItem *item : qAsConst (items))
  {
    /* skip invisible items */
    if (!item->isVisible())
      continue;

    /* get item */
    QSizeF size       = item->size();

    /* rules */
    bool willEverFit  = size.width() <= (canvasSize.width() - spacing());
    bool willFit      = size.width() <= (canvasSize.width() - pos.x());

    /* store highest item height */
    maxHeight = qMax (maxHeight, size.height());

    /* item's result rect */
    QRectF rect (pos.x(), pos.y(), size.width(), size.height());

    /* ------------------------------------------------------------------- */
    /* lambda's */
    /* ------------------------------------------------------------------- */

    auto nextLine = [&]
    {
      /* move to next line */
      pos.setX (0);
      pos.setY (pos.y() + maxHeight + spacing());

      /* update content height */
      content.setWidth (maxWidth);
      content.setHeight (content.height() + maxHeight);

      /* calc new item rect */
      rect  = QRectF (pos.x(), pos.y(), size.width(), size.height());

      /* move right next item */
      pos.setX (pos.x() + size.width() + spacing());

      /* reset max height */
      maxWidth  = 0;
      maxHeight = 0;
    };

    /* ------------------------------------------------------------------- */
    /* calc position based on rules */
    /* ------------------------------------------------------------------- */

    /* set item left most */
    if (!willEverFit)
    {
      /* store new max width */
      maxWidth  = qMax (maxWidth, size.width());

      /* update rect */
      rect      = QRectF (0, pos.y(), size.width(), size.height());

      /* move to next line */
      nextLine();
    }

    /* next line */
    else
      if (!willFit)
      {
        /* store new max width */
        maxWidth  = qMax (maxWidth, pos.x());

        /* move to next line */
        nextLine();
      }

    /* move right next item */
    else
      pos.setX (pos.x() + size.width() + spacing());

    /* ------------------------------------------------------------------- */
    /* set rect */
    /* ------------------------------------------------------------------- */

    qDebug().nospace() << "item " << i++ << " " << rect;
    item->setPosition (rect.topLeft());
  }

  /* store result content size */
  setContentWidth (content.width());
  setContentHeight (content.height());
  qDebug() << __func__ << canvasSize << "result:" << content;

  return nullptr;
}

/*-----------------------------------------*/

