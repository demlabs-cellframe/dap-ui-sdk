#include "ComboBoxListView.h"
#include "CustomButtonDelegate.h"
#include <QScroller>


ComboBoxListView::ComboBoxListView(QWidget *a_parent /*= nullptr*/)
    :ParentClass(a_parent)
{
#ifdef ANDROID
    this->setFocusPolicy(Qt::NoFocus);
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

#else
}

WidgetDelegateBase *ComboBoxListView::createWidgetDelegate()
{
    return new CustomButtonDelegate;
}
#endif


