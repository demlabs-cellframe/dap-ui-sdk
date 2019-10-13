#include "DapUiDialog.h"


/** @brief constructor
 *  @param a_parent object parent
 */
DapUiDialog::DapUiDialog(QWidget *a_parent):
    QFrame(a_parent),
    m_screenOverlaying(a_parent)
{
    qDebug() << "DapUiDialog created";

    m_screenOverlaying.setObjectName("DialogScreenOverlaying");
    m_screenOverlaying.setGeometry(a_parent->geometry());

    raise();
}

/** @brief Reimplemented QFrame::show slot. Show screen overlaying.
 */
void DapUiDialog::show()
{
    m_screenOverlaying.show();

    QFrame::show();
}

/** @brief Reimplemented QFrame::hide slot. Hide screen overlaying.
 */
void DapUiDialog::hide()
{
    QFrame::hide();

    m_screenOverlaying.hide();
}

/** @brief Reimplemented QFrame::resizeEvent method. Update dialog position.
 *  @param a_event Resize Event
 */
void DapUiDialog::resizeEvent(QResizeEvent *a_event)
{
     QFrame::resizeEvent(a_event);

     updatePosition();
}

/** @brief Update dialog position
 */
void DapUiDialog::updatePosition()
{
    QWidget *wtParent = qobject_cast<QWidget*>(parent());
    Q_ASSERT_X(wtParent, "DapUiDialog", "Wrong type of parent");

    int posX = (wtParent->width() - width()) / 2;
    int posY = (wtParent->height() - height()) / 2;
    move(posX, posY);
}
