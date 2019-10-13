#ifndef DAPUIANIMATIONSCREENABSTRACT_H
#define DAPUIANIMATIONSCREENABSTRACT_H

#include <QWidget>
#include <typeinfo>

#include "DapUiScreen.h"
#include "controls/AnimationChangingWidget.h"

class Ui_AnimationChangingScreen;

//=================================================================================
//                               Interface
//=================================================================================

/**
 * @brief Abstract class wich hold AnimationChangingWidget and can hold forms and child screens for all screen sizes and rotations.
 * @todo If stylesheet of AnimationChangingWidget is empty on the form, sizes will be wrong. Set wtylesheet to "*{}".
 */
class DapUIAnimationScreenAbstract : public DapUiScreen
{
    Q_OBJECT

protected:
    DapUIAnimationScreenAbstract(QObject *parent, QStackedWidget * a_sw);

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

    DapUIAnimationScreenAbstract *activateChildScreen(DapUiScreen *a_screen);

    AnimationChangingWidget *wgChangedScreen();

signals:
    void animationFinished();                       ///< Emits this signal after the animation transition has reached the end.

protected:
    virtual void initChangedScreen(QWidget * a_w);  ///< Find changingWidget and assign to m_wgChangedScreen
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) = 0; ///<pure virtual method. Must de reimplement it inherited classes

    AnimationChangingWidget *m_wgChangedScreen;     ///< Pointer to ChangingWidget controll

private:
    QMap<QString, DapUiScreen*> m_screens;          ///< Map with all screens that can be activated
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
T *DapUIAnimationScreenAbstract::createSubScreen(int a_index /*= -1*/)
{
    // if subscrin exist return
    T *newScreen = subScreen<T>();
    if (subScreen<T>())
        return newScreen;

    QStackedWidget *newSW = new QStackedWidget(m_wgChangedScreen);  // Create QStackedWidget for new screen and screen;
    newScreen = new T(this, newSW);                              // Create new screen

    //insert screen to m_screens and changing sreen widget
    m_screens.insert(typeid(T).name(), newScreen);
    m_wgChangedScreen->insertWidget(a_index, newScreen->sw());

    return newScreen;
}

/**
 * @brief Remove subscreen with type T
 * @details If Screen with type T is not found do nothing
 * @param T Removing screen type
 */
template<class T>
void DapUIAnimationScreenAbstract::removeSubscreen()
{
    DapUiScreen *screen = subScreen<T>(); // Find subscrin

    if (screen) { // If found ...
        // 1. remove from parent ChangingWidget, ...
        DapUIAnimationScreenAbstract *parentScreen = qobject_cast<DapUIAnimationScreenAbstract*> (screen->parent());
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
NewT *DapUIAnimationScreenAbstract::replaceSubscreen()
{
    OldT *oldScreen = subScreen<OldT>();  // Find subscrin

    if (oldScreen) {// If found ...
        //1. get index of oldScreen stacked widget
        DapUIAnimationScreenAbstract *parentScreen = qobject_cast<DapUIAnimationScreenAbstract*>(oldScreen->parent());
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
T *DapUIAnimationScreenAbstract::subScreen()
{
    QString screenTypeName =  typeid(T).name();

    T *foundScreen = qobject_cast<T*>(m_screens.value(screenTypeName, nullptr)); // Searh in m_screens

    //  If isn't found search in all subScreens recursively
    auto i = m_screens.constBegin();
    while (i != m_screens.constEnd() and !foundScreen) {
        DapUIAnimationScreenAbstract *curScreen = qobject_cast<DapUIAnimationScreenAbstract*>(i.value());
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
T *DapUIAnimationScreenAbstract::activateScreen()
{
    T *screen = subScreen<T>();    //< find subScreen with type T

    // If does not exist create new
    if (!screen)
        screen = createSubScreen<T>();

    // Activate all parent screens in line from found screen to this
    DapUIAnimationScreenAbstract *parent = qobject_cast<DapUIAnimationScreenAbstract*>(screen->parent());
    DapUiScreen *nextScreen = screen;
    //do while parent is not this screen
    while (nextScreen != this) {
        nextScreen = parent->activateChildScreen(nextScreen);
        parent = qobject_cast<DapUIAnimationScreenAbstract*>(parent->parent());
    }
    return screen;
}




#endif // DAPUIANIMATIONSCREENABSTRACT_H
