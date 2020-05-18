#include "ScreenOverlaying.h"

ScreenOverlaying::ScreenOverlaying(QMainWindow *a_parent)
    :QWidget(a_parent)
{
    setObjectName("ScreenOverlaying");
}

void ScreenOverlaying::setParent(QMainWindow *a_parent)
{
    QWidget::setParent(a_parent);

    if (this->isVisible() && m_blurEffect)
        acceptBlurEffect();
}


void ScreenOverlaying::showEvent(QShowEvent *event)
{
    this->acceptBlurEffect();

    QWidget *window = qobject_cast <QWidget*> (this->parent());

    if (window)
        this->resize(window->width(), window->height());

    QWidget::showEvent(event);
}

void ScreenOverlaying::hideEvent(QHideEvent *event)
{
    delete m_blurEffect;
    m_blurEffect = nullptr;

    QWidget::hideEvent(event);
}

void ScreenOverlaying::mousePressEvent(QMouseEvent *event)
{
    emit clicked();

    this->QWidget::mousePressEvent(event);
}

qreal ScreenOverlaying::blurRadius() const
{
    return m_blurRadius;
}
/// Setting a parameter for the BlurEffect.
/// @param a_blureRadius.
void ScreenOverlaying::setBlurRadius(qreal a_blurRadius)
{
    if (!m_blurEffect && a_blurRadius <= 0.0)
       return;

    m_blurRadius = a_blurRadius;

    if (this->isVisible())
        this->acceptBlurEffect();
}

qreal ScreenOverlaying::opacity() const
{
    QGraphicsOpacityEffect *opacityEffect = qobject_cast<QGraphicsOpacityEffect*>(this->graphicsEffect());

    if (!opacityEffect)
        return 1.0;

    return opacityEffect->opacity();
}

/// Setting a parameter for the OpacityEffect. The value must be between 0 and 1.
/// @param a_opocity.
void ScreenOverlaying::setOpacity(qreal a_opacity)
{
    QGraphicsOpacityEffect *opacityEffect = qobject_cast<QGraphicsOpacityEffect*>(this->graphicsEffect());

    if (!opacityEffect)
    {
        if (a_opacity >= 1.0)
            return;

        opacityEffect = new QGraphicsOpacityEffect(this);
        this->setGraphicsEffect(new QGraphicsOpacityEffect(this));
    }

    opacityEffect->setOpacity(a_opacity);
}

bool ScreenOverlaying::acceptBlurEffect()
{
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(this->parent());

    if (!mainWindow || !mainWindow->centralWidget() )
        return false;

    if (!m_blurEffect)
    {
        if (m_blurRadius <= 0.0)
            return false;

        m_blurEffect = new QGraphicsBlurEffect();
    }

    m_blurEffect->setBlurRadius(m_blurRadius);
    mainWindow->centralWidget()->setGraphicsEffect(m_blurEffect);

    return true;
}

