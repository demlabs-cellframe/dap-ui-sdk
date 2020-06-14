#include "ComboBoxListView.h"
#include "CustomButtonDelegate.h"
#include <QScroller>


ComboBoxListView::ComboBoxListView(QWidget *a_parent /*= nullptr*/)
    :WidgetDelegateListView(a_parent)
{
#ifdef ANDROID
    this->setFocusPolicy(Qt::NoFocus);
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
#endif
    this->setWidgetDelegateFactory(&CustomButtonDelegate::create); // Add creating item widgets
}
