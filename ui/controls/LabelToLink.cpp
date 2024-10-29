#include "LabelToLink.h"

LabelToLink::LabelToLink(QWidget *a_parent)
    :QLabel (a_parent)
{

}

void LabelToLink::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        QUrl url(property("URL").toString());
        QDesktopServices::openUrl(url);
    }
}
