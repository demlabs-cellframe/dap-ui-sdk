/* INCLUDES */
#include "dapqmlseparatoritem.h"
#include "dapqml-widgets/dapqmlstyle.h"
#include "style/qssmap.h"

#include <QPainter>

/* DEFS */
class QssMapAccessor : public DapStyle::QssMap
{
public:
  static DapStyle::QssItem *item (const QString &a_name)
  {
    return DapStyle::QssMap::item (a_name);
  }
};

/* VARS */
static DapQmlStyle *s_styleRedrawListener = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlSeparatorItem::DapQmlSeparatorItem (QQuickItem *a_parent)
  : QQuickPaintedItem (a_parent)
  , _root (nullptr)
{
  if (s_styleRedrawListener == nullptr)
    s_styleRedrawListener = new DapQmlStyle;
  connect (s_styleRedrawListener, &DapQmlStyle::redrawRequested,
           this, [this] { update(); });
}

/********************************************
 * METHOD
 *******************************************/

void DapQmlSeparatorItem::setRootItem (QQuickItem *a_root)
{
  _root = a_root;
  update();
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapQmlSeparatorItem::paint (QPainter *a_painter)
{
  /* get coordinates */
  //auto recct = boundingRect();
  qreal pos     = mapToItem (_root, QPointF()).y();
  int iPos      = int (pos);
  qreal height  = this->height() / 2;

  /* get separator color */
  QColor color;
  auto item = QssMapAccessor::item ("sep-color");
  for (auto i = item->cbegin(), e = item->cend(); i != e; i++)
    if (i.key() == "color")
    {
      color = i.value().value<QColor>();
      break;
    }

  //auto clr  = color.name (QColor::HexArgb);
  auto rect = QRectF(
    0, 1 + (pos - qreal (iPos)),
    width(), (height > 1 ? height : 1));
  a_painter->setPen (color);
  a_painter->drawRect (rect);
}

/*-----------------------------------------*/
