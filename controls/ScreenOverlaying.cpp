#include "ScreenOverlaying.h"

ScreenOverlaying::ScreenOverlaying(QMainWindow *a_parent)
    :QWidget(a_parent)
{
    setObjectName("ScreenOverlaying");

    this->setVisible(false);
}

void ScreenOverlaying::showEvent(QShowEvent *event)
{
    if(parent())
    {
        qobject_cast <QMainWindow*> (this->parent())->centralWidget()->setGraphicsEffect(m_blurEffect);

        setGeometry(0,0,qobject_cast <QWidget*> (this->parent())->width(),qobject_cast <QWidget*> (this->parent())->height());
    }
    QWidget::showEvent(event);
}

void ScreenOverlaying::hideEvent(QHideEvent *event)
{    
    delete  qobject_cast <QMainWindow*> (this->parent())->centralWidget()->graphicsEffect();
    QWidget::hideEvent(event);
}
void ScreenOverlaying::setBlurRadius(int &a_blurRadius)
{
    if(!m_blurEffect)
        m_blurEffect = new QGraphicsBlurEffect();
    m_blurEffect->setBlurRadius(a_blurRadius);
}

void ScreenOverlaying::setOpacity(qreal &a_opacity)
{
    if(a_opacity < 0 || a_opacity > 1)
        return;

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(a_opacity);

    if(graphicsEffect())
    {
        delete graphicsEffect();
    }
    setGraphicsEffect(opacityEffect);
}

