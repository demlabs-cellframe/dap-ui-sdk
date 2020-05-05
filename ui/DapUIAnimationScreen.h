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
#include "DapUIAnimationScreenAbstract.h"

/**
 * @brief Class wich hold full screen AnimationChangingWidget and can hold child screens
 */
class DapUIAnimationScreen : public DapUIAnimationScreenAbstract
{
    Q_OBJECT

public:
    DapUIAnimationScreen(QObject *parent, QStackedWidget *a_sw);

protected:
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation);

};



//=======================================================================
//                            Default Form
//=======================================================================

class Ui_AnimationChangingScreen
{
public:
    AnimationChangingWidget *changingScreen;
    QHBoxLayout *horizontalLayout;

    void setupUi(QWidget *a_wgScreen)
    {
        if (a_wgScreen->objectName().isEmpty())
            a_wgScreen->setObjectName(QStringLiteral("AnimationScreenFullScreen"));
 //       a_wgScreen->resize(1280, 804);
        horizontalLayout = new QHBoxLayout(a_wgScreen);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("centralLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        a_wgScreen->setLayout(horizontalLayout);

        changingScreen = new AnimationChangingWidget(a_wgScreen);
        changingScreen->setObjectName(QStringLiteral("changingScreen"));
        changingScreen->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        changingScreen->setStyleSheet("#changingScreen{}");
        horizontalLayout->addWidget(changingScreen);
    } // setupUi

};
