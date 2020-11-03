#include "ComboBoxListView.h"
#include "CustomButtonDelegate.h"
#include <QScroller>


ComboBoxListView::ComboBoxListView(QWidget *a_parent /*= nullptr*/)
    :WidgetDelegateListView(a_parent)
{
#ifdef ANDROID
    this->setFocusPolicy(Qt::NoFocus);
    QScroller* scroller = QScroller::scroller(this);
    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    scroller->setScrollerProperties(sp);
    scroller->grabGesture(this, QScroller::LeftMouseButtonGesture);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
#endif
    this->setWidgetDelegateFactory(&CustomButtonDelegate::create); // Add creating item widgets
}
