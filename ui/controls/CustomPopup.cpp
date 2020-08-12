#include "CustomPopup.h"
#include "AppStyleSheetHandler.h"

/** @brief PopUp with form and screen overlaying
*/
CustomPopup::CustomPopup(QWidget *a_parent /*= nullptr*/)
    : AdaptiveWidget(Utils::findParent<QMainWindow*>(a_parent))
{
    this->setObjectName("stwCustomPopup");
}

void CustomPopup::setParent(QWidget *a_parent)
{
    if (m_screenOverlaying)
    {
        QMainWindow *parentMainWindow = Utils::findParent<QMainWindow*>(this->parent());
        Q_ASSERT(parentMainWindow);

        m_screenOverlaying->setParent(parentMainWindow);
    }

    this->AdaptiveWidget::setParent(a_parent);
    this->updatePosition();
}

qreal CustomPopup::screenOverlayingBlurRadius() const
{
    if (!m_screenOverlaying)
        return 0.0;

    return m_screenOverlaying->blurRadius();
}

void CustomPopup::setScreenOverlayingOpacity(qreal a_opacity)
{
    if (!m_screenOverlaying && a_opacity < 1.0)
    {
        createScreenOverlaying();
        if (this->windowType() == Qt::Widget || this->windowType() == Qt::Desktop)
            m_windowType = Qt::Dialog;
    }

    m_screenOverlaying->setOpacity(a_opacity);
}

qreal CustomPopup::screenOverlayingOpacity() const
{
    if (!m_screenOverlaying)
        return 0.0;

    return m_screenOverlaying->opacity();
}

void CustomPopup::setScreenOverlayingBlurRadius(qreal a_blurRadius)
{
    if (!m_screenOverlaying && a_blurRadius > 0.0)
    {
        if (this->windowType() == Qt::Desktop)
        {
            qWarning() << "Can't assign screenOverlaying to CustomPopup with Desktop windowType";
            return;
        }

        createScreenOverlaying();

        if (this->windowType() == Qt::Widget)
            m_windowType = Qt::Dialog;
    }

    m_screenOverlaying->setBlurRadius(a_blurRadius);
}

/** @brief Reimplemented QFrame::resizeEvent method. Update dialog position.
 *  @param a_event Resize Event
 */
void CustomPopup::resizeEvent(QResizeEvent *a_event)
{
     AdaptiveWidget::resizeEvent(a_event);

     updatePosition();
}

/** @brief Update dialog position
 */
void CustomPopup::updatePosition()
{
    QWidget *wgtParent = qobject_cast<QWidget*>(parent());

    if (!wgtParent)
        return;

    switch (windowType()) {
        case Qt::Desktop:
        {
            this->move(0,0);
            this->resize(wgtParent->size());
            break;
        }
        case Qt::Dialog:
        case Qt::Popup:
        {
            int posX = (wgtParent->width() - width()) / 2;
            int posY = (wgtParent->height() - height()) / 2;
            move(posX, posY);
            break;
        }
        case Qt::ToolTip:
        default: return;
    }
}

void CustomPopup::createScreenOverlaying()
{
    QMainWindow *parentMainWindow = Utils::findParent<QMainWindow*>(this->parent());

    if (!parentMainWindow)
        return;

    m_screenOverlaying = new ScreenOverlaying(parentMainWindow);
    this->raise();
}

Qt::WindowType CustomPopup::windowType() const
{
    return m_windowType;
}

void CustomPopup::setWindowType(Qt::WindowType a_windowType)
{
    if (m_windowType == a_windowType)
        return;

    if (a_windowType == Qt::Widget || a_windowType == Qt::Desktop)
    {
        delete m_screenOverlaying;
        m_screenOverlaying = nullptr;
    }
    else
    {
        if (!m_screenOverlaying)
            createScreenOverlaying();

         m_screenOverlaying->disconnect();

        if (a_windowType == Qt::Popup || a_windowType == Qt::ToolTip)
            connect(m_screenOverlaying, SIGNAL(clicked()), SLOT(hide()));
    }

    m_windowType = a_windowType;
    updatePosition();
}

void CustomPopup::setVisible(bool a_visible)
{
    if (m_screenOverlaying)
        m_screenOverlaying->setVisible(a_visible);

    this->AdaptiveWidget::setVisible(a_visible);

    if (a_visible)
        emit this->opened();
    else
        emit this->closed();
}

