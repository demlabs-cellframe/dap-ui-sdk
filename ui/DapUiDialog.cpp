/*
* Authors:
* Dmitriy Gerasimov <naeper@demlabs.net>
* Cellframe       https://cellframe.net
* DeM Labs Inc.   https://demlabs.net
* Copyright  (c) 2017-2019
* All rights reserved.

This file is part of CellFrame SDK the open source project

CellFrame SDK is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CellFrame SDK is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with any CellFrame SDK based project.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DapUiDialog.h"


/** @brief constructor
 *  @param a_parent object parent
 */
DapUiDialog::DapUiDialog(QWidget *a_parent):
    QFrame(a_parent),
    m_screenOverlaying()
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
