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

#include "DefaultMultiScreen.h"

const QString DefaultMultiScreen::SCREEN_NAME("FullScreen");

/**
 * @brief Overloaded constructor.
 * @param a_parent Parent.
 * @param a_sw Application window stack.
 */
DefaultMultiScreen::DefaultMultiScreen(QWidget *a_parent)
    : MultiScreenAbstract(a_parent)
{
    create<Ui_DefaultMultiScreen>();
}

QString DefaultMultiScreen::screenName()
{
    return DefaultMultiScreen::SCREEN_NAME;
}

/**
 * @brief Form initialization.
 * @param a_w Window GUI widget.
 * @param a_rotation Device display orientation.
 */
void DefaultMultiScreen::initVariantUi(QWidget * a_widget)
{
    // Initializing pointers to window widgets
    this->initChangingWidget(a_widget);
}
