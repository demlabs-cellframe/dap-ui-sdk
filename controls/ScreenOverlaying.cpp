#include "ScreenOverlaying.h"

ScreenOverlaying::ScreenOverlaying(QMainWindow *a_parent)
    :QWidget(a_parent),
      m_blureEffect(new QGraphicsBlurEffect())
{
    setObjectName("ScreenOverlaying");

    this->setVisible(false);
}

void ScreenOverlaying::showEvent(QShowEvent *event)
{
    qobject_cast <QMainWindow*> (this->parent())->centralWidget()->setGraphicsEffect(m_blureEffect);

    setGeometry(0,0,qobject_cast <QWidget*> (this->parent())->width(),qobject_cast <QWidget*> (this->parent())->height());
    QWidget::showEvent(event);
}

void ScreenOverlaying::hideEvent(QHideEvent *event)
{    
    delete  qobject_cast <QMainWindow*> (this->parent())->centralWidget()->graphicsEffect();
    QWidget::hideEvent(event);
}
void ScreenOverlaying::setBlurRadius(int &a_blurRadius)
{
    m_blureEffect->setBlurRadius(a_blurRadius);
}

void ScreenOverlaying::setOpacity(qreal &a_opacity)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(a_opacity);

    setGraphicsEffect(opacityEffect);
}

