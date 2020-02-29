#include "CustomComboBox.h"
#include <QDebug>
/** @brief constructor
 *  @param a_parent object parent
 */
CustomComboBox::CustomComboBox(QWidget *parent):
    QComboBox (parent)
{
    m_styledshadow = new StyledDropShadowEffect(this);
}

/** @brief Reimplemented QComboBox::setObjectName method. Updates stylesheets.
 *  @param text Text
 */
void CustomComboBox::setObjectName(const QString &name)
{
    QComboBox::setObjectName(name);
    m_styledshadow->updateStyleProperties();
    setGraphicsEffect(m_styledshadow);
}

void CustomComboBox::showPopup()
{
    qDebug() << "show Popup";
    emit showCustomWindow();
}
/** @brief Reimplemented QComboBox::enterEvent is sent to the widget when the mouse cursor enters the widget.
 *  @param event
 */
void CustomComboBox:: enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_styledshadow->updateStyle(HOVER_SHADOW);
    setGraphicsEffect(m_styledshadow);

    setProperty("hoverState",1);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

/** @brief Reimplemented QComboBox::leaveEvent is sent to the widget when the mouse cursor leaves the widget.
 *  @param event
 */
void CustomComboBox::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_styledshadow->updateStyle(DEFAULT_SHADOW);
    setGraphicsEffect(m_styledshadow);

    setProperty("hoverState",0);
    style()->unpolish(this);
    style()->polish(this);
    update();
}
