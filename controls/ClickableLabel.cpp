#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel (parent)
{

}

ClickableLabel::~ClickableLabel()
{

}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked();
}
