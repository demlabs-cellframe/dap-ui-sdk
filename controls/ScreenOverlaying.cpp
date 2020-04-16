#include "ScreenOverlaying.h"

ScreenOverlaying::ScreenOverlaying(QMainWindow *a_parent)
    :QWidget(a_parent)
{
    setObjectName("ScreenOverlaying");
    m_blureEffect = a_parent->centralWidget();

    this->setVisible(false);
}

void ScreenOverlaying::showEvent(QShowEvent *event)
{
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(m_blureEffect);
    blurEffect->setBlurRadius(m_blurRadius);
    m_blureEffect->setGraphicsEffect(blurEffect);

    setGeometry(0,0,m_blureEffect->width(),m_blureEffect->height());
    QWidget::showEvent(event);
}

void ScreenOverlaying::hideEvent(QHideEvent *event)
{
    delete m_blureEffect->graphicsEffect();
    QWidget::hideEvent(event);
}
void ScreenOverlaying::setBlurRadius(int &a_blurRadius)
{
    m_blurRadius = a_blurRadius;
}

void ScreenOverlaying::setOpacity(qreal &a_opacity)
{
    m_opacity = a_opacity;

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(m_opacity);

    setGraphicsEffect(opacityEffect);
}
