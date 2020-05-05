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
#pragma once
#include <QtDebug>
#include <QAbstractButton>
#include <QStyle>

#include "DapUiScreen.h"
#include "controls/ScreenOverlaying.h"

/** @brief Dialog with form and screen overlaying
*/
class DapUiDialog : public QFrame
{
    Q_OBJECT
public:
    DapUiDialog(QWidget * a_parent);
    virtual ~DapUiDialog() { qDebug() << "DapUiDialog destroyed"; }

    template<class TUi>
    void createDialog();

    virtual void initUi() = 0; ///< Mast be reimplemented

public slots:
    void show();
    virtual void hide();

protected:
    virtual void resizeEvent(QResizeEvent *a_event);

    void updatePosition();

private:

    ScreenOverlaying m_screenOverlaying; ///<Screen Overlaying
};

//=================================================================================
//                               Interface
//=================================================================================
/** @brief
 *  @param TUi Type of UI form
 */
template<class TUi>
void DapUiDialog::createDialog()
{
    static TUi *ui;
    if ( ui != nullptr)
            delete ui;
    ui = new TUi;

    ui->setupUi(this);

    initUi();
}


