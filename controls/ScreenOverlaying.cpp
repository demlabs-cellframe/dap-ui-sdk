#include "ScreenOverlaying.h"


ScreenOverlaying::ScreenOverlaying(QMainWindow *a_parent)
    :QWidget(a_parent),
    blurRadius(0),
    opacity(0)
{
    setObjectName("ScreenBlureOpocityEffect");
    m_blurParent = a_parent->centralWidget();
    m_opacityParent = a_parent;

    this->setVisible(false);
}

ScreenOverlaying::ScreenOverlaying(QWidget *blure_parent, QWidget *opacity_parent)
    :QWidget(opacity_parent),
    blurRadius(0),
    opacity(0)
{
    setObjectName("ScreenBlureOpocityEffect");
    m_blurParent = blure_parent;
    m_opacityParent = opacity_parent;

    this->setVisible(false);
}

void ScreenOverlaying::showEvent(QShowEvent *event)
{
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(m_blurParent);
    blurEffect->setBlurRadius(blurRadius);
    m_blurParent->setGraphicsEffect(blurEffect);

    setGeometry(0,0,m_opacityParent->width(),m_opacityParent->height());
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(opacity);

    setGraphicsEffect(opacityEffect);


    QWidget::showEvent(event);
}

void ScreenOverlaying::hideEvent(QHideEvent *event)
{
    delete m_blurParent->graphicsEffect();
    delete graphicsEffect();
    QWidget::hideEvent(event);
}
void ScreenOverlaying::setBlurRadius(int &a_blurRadius)
{
    blurRadius = a_blurRadius;
}

void ScreenOverlaying::setOpacity(qreal &a_opacity)
{
    opacity = a_opacity;
}
