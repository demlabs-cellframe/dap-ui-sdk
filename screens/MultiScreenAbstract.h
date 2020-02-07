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

#include <QWidget>
#include <typeinfo>

#include "DapUiScreen.h"
#include "AdaptiveScreen.h"
#include "controls/AnimationChangingWidget.h"


//=================================================================================
//                               Interface
//=================================================================================

/**
 * @brief Abstract class wich hold AnimationChangingWidget and can hold forms and child screens for all screen sizes and rotations.
 * @todo If stylesheet of AnimationChangingWidget is empty on the form, sizes will be wrong. Set wtylesheet to "*{}".
 */
class MultiScreenAbstract : public AdaptiveScreen
{
    Q_OBJECT

protected:
    MultiScreenAbstract(QWidget *parent);

public:
    template<class T>
    T *createSubScreen(int a_index = -1);

    template<class T>
    void removeSubscreen();

    template<class OldT, class NewT>
    NewT *replaceSubscreen();

    template<class T>
    T *subScreen();

    template<class T>
    T *activateScreen();

    MultiScreenAbstract *activateChildScreen(AdaptiveScreen *a_screen);

    AnimationChangingWidget *wgChangedScreen();

signals:
    void animationFinished();                       ///< Emits this signal after the animation transition has reached the end.

protected:
    virtual void initChangedScreen(QWidget * a_w);  ///< Find changingWidget and assign to m_wgChangedScreen
    virtual void initVariantUi(QWidget * a_widget) = 0; ///<pure virtual method. Must de reimplement it inherited classes
    virtual QString screenName() = 0;

    AnimationChangingWidget *m_wgChangedScreen;     ///< Pointer to ChangingWidget controll

private:
    QMap<QString, AdaptiveScreen*> m_screens;          ///< Map with all screens that can be activated
};







//=================================================================================
//                               Realization
//=================================================================================


/**
 * @brief Create screen with type T if not exist
 * @param T new screen type
 * @return Existing or created screen
 */
template<class T>
T *MultiScreenAbstract::createSubScreen(int a_index /*= -1*/)
{
    // if subscrin exist return
    T *newScreen = subScreen<T>();
    if (subScreen<T>())
        return newScreen;

    newScreen = new T(this);                              // Create new screen

    //insert screen to m_screens and changing sreen widget
    m_screens.insert(typeid(T).name(), newScreen);
    m_wgChangedScreen->insertWidget(a_index, newScreen);

    return newScreen;
}

/**
 * @brief Remove subscreen with type T
 * @details If Screen with type T is not found do nothing
 * @param T Removing screen type
 */
template<class T>
void MultiScreenAbstract::removeSubscreen()
{
    DapUiScreen *screen = subScreen<T>(); // Find subscrin

    if (screen) { // If found ...
        // 1. remove from parent ChangingWidget, ...
        MultiScreenAbstract *parentScreen = qobject_cast<MultiScreenAbstract*> (screen->parent());
        parentScreen->wgChangedScreen()->removeWidget(screen->sw());
        // 2. delete stackedWidget and screen
        delete screen->sw();
        delete screen;
        // 3. delete from Map with screens pointers
        m_screens.remove(typeid(T).name());
    }
    // if isn't found do nothing
}

/**
 * @brief Remove screen with type OldT and create subscreen with type NewT in the place of deleted
 * @param OldT Removed screen type
 * @param NewT New screen type
 * @return New screen
 */
template<class OldT, class NewT>
NewT *MultiScreenAbstract::replaceSubscreen()
{
    OldT *oldScreen = subScreen<OldT>();  // Find subscrin

    if (oldScreen) {// If found ...
        //1. get index of oldScreen stacked widget
        MultiScreenAbstract *parentScreen = qobject_cast<MultiScreenAbstract*>(oldScreen->parent());
        int index = parentScreen->wgChangedScreen()->indexOf(oldScreen->sw());
        //2. remove oldScreen, ...
        parentScreen->removeSubscreen<OldT>();
        //3. create new subscreen in the place of deleted
        return parentScreen->createSubScreen<NewT>(index);
    }
    // if isn't found just create new
    return createSubScreen<NewT>();

}

/**
 * @brief Find subScreen with type T in child screens recursively
 * @param T searching screen type
 * @return subScreen with type T
 */
template<class T>
T *MultiScreenAbstract::subScreen()
{
    QString screenTypeName =  typeid(T).name();

    T *foundScreen = qobject_cast<T*>(m_screens.value(screenTypeName, nullptr)); // Searh in m_screens

    //  If isn't found search in all subScreens recursively
    auto i = m_screens.constBegin();
    while (i != m_screens.constEnd() and !foundScreen) {
        MultiScreenAbstract *curScreen = qobject_cast<MultiScreenAbstract*>(i.value());
        //if i.value() is DapUIAnimationScreenAbstract object search in all subScreens
        if (curScreen)
            foundScreen = curScreen->subScreen<T>();
        ++i;
    }

    return foundScreen;
}

/**
 * @brief Find subScreen with type T and activate all child screens in line from this to found screen
 * @param T activating screen type
 * @return activated subScreen with type T
 */
template<class T>
T *MultiScreenAbstract::activateScreen()
{
    T *screen = subScreen<T>();    //< find subScreen with type T

    // If does not exist create new
    if (!screen)
        screen = createSubScreen<T>();

    // Activate all parent screens in line from found screen to this
    MultiScreenAbstract *parent = qobject_cast<MultiScreenAbstract*>(screen->parent());
    AdaptiveScreen *nextScreen = screen;
    //do while parent is not this screen
    while (nextScreen != this) {
        nextScreen = parent->activateChildScreen(nextScreen);
        parent = qobject_cast<MultiScreenAbstract*>(parent->parent());
    }
    return screen;
}
