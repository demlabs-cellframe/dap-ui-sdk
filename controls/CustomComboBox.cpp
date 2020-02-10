#include "CustomComboBox.h"

/** @brief constructor
 *  @param a_parent object parent
 */
CustomComboBox::CustomComboBox(QWidget *parent):
    QComboBox (parent)
{

}

/** @brief Reimplemented QComboBox::enterEvent is sent to the widget when the mouse cursor enters the widget.
 *  @param event
 */
void CustomComboBox:: enterEvent(QEvent *event)
{
    Q_UNUSED(event);

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

    setProperty("hoverState",0);
    style()->unpolish(this);
    style()->polish(this);
    update();
}
