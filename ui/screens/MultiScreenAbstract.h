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
    T *createSubScreenIfNotExist(int a_index = -1);

    template<class T>
    void removeSubscreen();

    void removeSubscreenAfterAnimationFinished(AdaptiveScreen* a_screen);
    void removeSubscreen(AdaptiveScreen* a_screen);

    template<class OldT, class NewT>
    NewT *replaceSubscreen();

    template<class T>
    T *refreshSubscreen();

    template<class T>
    T *subScreen();

    template<class T>
    T *activateScreen(int a_index = -1);

    AdaptiveScreen* activeScreen();
    QString activeScreenName();

    MultiScreenAbstract *activateChildScreen(AdaptiveScreen *a_screen);
    static MultiScreenAbstract *parentMultiscreen(AdaptiveScreen *a_screen);

    AnimationChangingWidget *changingWidget();

signals:
    void animationFinished();                       ///< Emits this signal after the animation transition has reached the end.

protected:
    virtual AnimationChangingWidget* findChangingWidget(QWidget *a_widget = nullptr);
    virtual void initChangingWidget(QWidget *a_widget = nullptr);  ///< Find changingWidget and assign to m_wgChangedScreen
    virtual void initVariantUi(QWidget * a_widget) = 0; ///<pure virtual method. Must de reimplement it inherited classes
    virtual QString screenName() = 0;

    AnimationChangingWidget *m_wgtChangingWidget = nullptr;     ///< Pointer to ChangingWidget controll

private slots:
    void afterAnimationScreensRemoval();

private:
    template<class T>
    T *createSubScreen(int a_index = -1);

    QMultiMap<QString, AdaptiveScreen*> m_screens;          ///< Map with all screens that can be activated
    QList<AdaptiveScreen*> m_screensForRemoval;
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
T *MultiScreenAbstract::createSubScreenIfNotExist(int a_index /*= -1*/)
{
    // if subscrin exist return
    T *newScreen = subScreen<T>();
    if (subScreen<T>())
        return newScreen;

    newScreen = new T(this);                              // Create new screen

    //insert screen to m_screens and changing sreen widget
    m_screens.insert(newScreen->screenName(), newScreen);
    this->changingWidget()->insertWidget(a_index, newScreen);

    return newScreen;
}

/**
 * @brief Create screen with type T withowt existing check
 * @param T new screen type
 * @return Existing or created screen
 */
template<class T>
T *MultiScreenAbstract::createSubScreen(int a_index /*= -1*/)
{
    T *newScreen = new T(this);                              // Create new screen

    //insert screen to m_screens and changing sreen widget
    m_screens.insertMulti(newScreen->screenName(), newScreen);
    this->changingWidget()->insertWidget(a_index, newScreen);

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
    AdaptiveScreen *screen = subScreen<T>(); // Find subscreen

    if (screen) { // If found ...
        this->removeSubscreen(screen);
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
    OldT *oldScreen = this->subScreen<OldT>();  // Find subscrin

    if (oldScreen) {// If found ...
        MultiScreenAbstract *parentScreen = MultiScreenAbstract::parentMultiscreen(oldScreen);
        int index = parentScreen->changingWidget()->indexOf(oldScreen);

        parentScreen->removeSubscreen<OldT>();
        return parentScreen->createSubScreen<NewT>(index);
    }
    // if isn't found just create new
    return createSubScreen<NewT>();
}

/**
 * @brief Refresh subscreen with type T
 *
 * @details Create subscreen with type T. If is active the same type run transition to new screen. Delete old screen after that
 * @param OldT Removed screen type
 * @param NewT New screen type
 * @return New screen
 */
template<class T>
T *MultiScreenAbstract::refreshSubscreen()
{
    AdaptiveScreen* existingScreen = this->subScreen<T>();
    if (!existingScreen)
        return nullptr;

    MultiScreenAbstract* parentMultiScreen = MultiScreenAbstract::parentMultiscreen(existingScreen);

    if (parentMultiScreen->activeScreen() == existingScreen)
    {
        int index = parentMultiScreen->changingWidget()->currentIndex();

        T* newScreen = parentMultiScreen->createSubScreen<T>(index + 1);

        parentMultiScreen->removeSubscreenAfterAnimationFinished(existingScreen);
        parentMultiScreen->activateChildScreen(newScreen);
        return newScreen;
    }
    else
    {
        return this->replaceSubscreen<T, T>();
    }
}

/**
 * @brief Find subScreen with type T in child screens recursively
 * @param T searching screen type
 * @return subScreen with type T
 */
template<class T>
T *MultiScreenAbstract::subScreen()
{
    T *foundScreen = qobject_cast<T*>(m_screens.value(T::SCREEN_NAME, nullptr));
    if (foundScreen)
        return foundScreen;

    auto i = m_screens.constBegin();
    while (i != m_screens.constEnd() and !foundScreen)
    {
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
 * @details if doesn't, exists will be created at a_index
 * @param T activating screen type
 * @param a_index index of new screen if it doesn't exist
 * @return activated subScreen with type T
 */
template<class T>
T *MultiScreenAbstract::activateScreen(int a_index /* = -1*/)
{
    T *screen = subScreen<T>();    //< find subScreen with type T
    MultiScreenAbstract *parentMultiScreen;

    if (screen)
        parentMultiScreen = MultiScreenAbstract::parentMultiscreen(screen);
    else
    {
        screen = this->createSubScreen<T>(a_index);
        parentMultiScreen = this;
    }

    // Activate all parent screens in line from found screen to this
    AdaptiveScreen *nextScreen = screen;
    //do while parent is not this screen
    while (nextScreen != this) {
        nextScreen = parentMultiScreen->activateChildScreen(nextScreen);
        parentMultiScreen = MultiScreenAbstract::parentMultiscreen(parentMultiScreen);
    }
    return screen;
}
