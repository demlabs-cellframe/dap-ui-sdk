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

#include "MultiScreenAbstract.h"

/**
 * @brief Overloaded constructor.
 * @param a_parent Parent.
 * @param a_sw Application window stack.
 */
MultiScreenAbstract::MultiScreenAbstract(QWidget *parent)
    :AdaptiveScreen(parent)
{
    connect(this, SIGNAL(animationFinished()), SLOT(afterAnimationScreensRemoval()));
}

void MultiScreenAbstract::removeSubscreen(AdaptiveScreen *a_screen)
{
    MultiScreenAbstract *parentScreen = MultiScreenAbstract::parentMultiscreen(a_screen);
    parentScreen->changingWidget()->removeWidget(a_screen);
    parentScreen->m_screens.remove(a_screen->screenName(), a_screen);
    parentScreen->m_screensForRemoval.removeAll(a_screen);
    delete a_screen;
}

void MultiScreenAbstract::removeSubscreenAfterAnimationFinished(AdaptiveScreen* a_screen)
{
    if (a_screen && !m_screensForRemoval.contains(a_screen))
        this->m_screensForRemoval.append(a_screen);
}

AdaptiveScreen *MultiScreenAbstract::activeScreen()
{
    QWidget* l_activeWidget = this->changingWidget()->currentWidget();
    return qobject_cast<AdaptiveScreen*>(l_activeWidget);
}

QString MultiScreenAbstract::activeScreenName()
{
    AdaptiveScreen* activeScreen = this->activeScreen();

    if (!activeScreen)
        return QString();

    return this->activeScreen()->screenName();
}

/**
 * @brief Activate child a_screen. a_screen must exist
 * @param a_screen pointer to child screen
 * @return pointer to this screen (parent)
 */
MultiScreenAbstract *MultiScreenAbstract::activateChildScreen(AdaptiveScreen *a_screen)
{
    Q_ASSERT_X(MultiScreenAbstract::parentMultiscreen(a_screen) == this, "activateDescendantScreen", "screen is not a child of this screen");

    m_wgtChangingWidget->setCurrentWidget(a_screen);

    return (this);
}

MultiScreenAbstract *MultiScreenAbstract::parentMultiscreen(AdaptiveScreen *a_screen)
{
    MultiScreenAbstract *l_parentMultiscr;

    for (QObject *parentObj = a_screen->parent(); parentObj; parentObj = parentObj->parent())
    {
        l_parentMultiscr = qobject_cast<MultiScreenAbstract*>(parentObj);
        if (l_parentMultiscr)
            return l_parentMultiscr;
    }
    return nullptr;
}

/**
 * @brief Getter for m_wgChangingScreen
 * @return pointer to AnimationChangingWidget
 */
AnimationChangingWidget *MultiScreenAbstract::changingWidget()
{
    return m_wgtChangingWidget;
}

/**
 * @brief Find and Return AnimationChangingWidget
 * @param a_widget pointer widget in which will be founded ChangedWidget
 */
AnimationChangingWidget *MultiScreenAbstract::findChangingWidget(QWidget *a_widget /*= nullptr*/)
{
    // initialize default a_widget
    if (!a_widget)
        a_widget = variant(ScreenInfo::currentRotation());

    return a_widget->findChild<AnimationChangingWidget*>();
}

/**
 * @brief Initialize m_wgChangedScreen. Must be called by initUi of inheritor class
 * @param a_widget pointer widget in which will be founded ChangedWidget
 */
void MultiScreenAbstract::initChangingWidget(QWidget *a_widget /*= nullptr*/)
{
    // initialize default a_widget
    if (!a_widget)
        a_widget = variant(ScreenInfo::currentRotation());

    // Initializing pointers to window widgets
    m_wgtChangingWidget = this->findChangingWidget(a_widget);
    Q_ASSERT(m_wgtChangingWidget);

    connect(m_wgtChangingWidget, &AnimationChangingWidget::animationFinished, [this]{
        emit animationFinished();
    });
}

void MultiScreenAbstract::afterAnimationScreensRemoval()
{
    while (!m_screensForRemoval.isEmpty())
    {
        this->removeSubscreen(m_screensForRemoval.first());
    }
}

