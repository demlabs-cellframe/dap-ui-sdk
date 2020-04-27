#include "CustomPopup.h"


/** @brief PopUp with form and screen overlaying
*/
CustomPopup::CustomPopup(QMainWindow *a_parent /*= nullptr*/)
    : AdaptiveWidget(a_parent)
{
    this->setObjectName("stwCustomPopup");
}


/** @brief Reimplemented QFrame::show slot. Show screen overlaying.
 */
void CustomPopup::show()
{
    if (m_screenOverlaying)
        m_screenOverlaying->show();

    AdaptiveWidget::show();
}

/** @brief Reimplemented QFrame::hide slot. Hide screen overlaying.
 */
void CustomPopup::hide()
{
    QFrame::hide();

    if (m_screenOverlaying)
        m_screenOverlaying->hide();
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
    Q_ASSERT_X(wgtParent, "DapUiDialog", "Wrong type of parent");

    int posX = (wgtParent->width() - width()) / 2;
    int posY = (wgtParent->height() - height()) / 2;
    move(posX, posY);
}

void CustomPopup::createScreenOverlaying()
{
    QMainWindow *parentMainWindow = qobject_cast<QMainWindow*>(this->parent());
    m_screenOverlaying = new ScreenOverlaying(parentMainWindow);

    this->raise();
}

Qt::WindowType CustomPopup::windowType() const
{
}

void CustomPopup::setWindowType(Qt::WindowType a_windowType)
{
    if (m_windowType == a_windowType)
        return;

    if (a_windowType == Qt::Widget)
    {
        delete m_screenOverlaying;
        m_screenOverlaying = nullptr;
    }
    else
    {
        if (!m_screenOverlaying)
            createScreenOverlaying();

        if (a_windowType == Qt::Dialog)
            m_screenOverlaying->disconnect();
        else if (a_windowType == Qt::Popup)
            connect(m_screenOverlaying, SIGNAL(clicked()), SLOT(hide()));
    }

    m_windowType = a_windowType;
}

void CustomPopup::setScreenOverlayingOpacity(qreal a_opacity)
{
    if (!m_screenOverlaying && a_opacity < 1.0)
    {
        createScreenOverlaying();
        m_windowType = Qt::Dialog;
    }

    m_screenOverlaying->setOpacity(a_opacity);
}


void CustomPopup::setScreenOverlayingBlurRadius(qreal a_blurRadius)
{
    if (!m_screenOverlaying && a_blurRadius > 0.0)
    {
        createScreenOverlaying();
        m_windowType = Qt::Dialog;
    }

    m_screenOverlaying->setBlurRadius(a_blurRadius);
}
