#include "DapUIAnimationScreen.h"

/**
 * @brief Overloaded constructor.
 * @param a_parent Parent.
 * @param a_sw Application window stack.
 */
DapUIAnimationScreen::DapUIAnimationScreen(QObject *parent, QStackedWidget *a_sw)
    :DapUIAnimationScreenAbstract(parent, a_sw)
{
    #ifdef DAP_PLATFORM_DESKTOP
        create<Ui_AnimationChangingScreen, Ui_AnimationChangingScreen, Ui_AnimationChangingScreen>();
    #else
        create<Ui_AnimationChangingScreen, Ui_AnimationChangingScreen, Ui_AnimationChangingScreen, Ui_AnimationChangingScreen, Ui_AnimationChangingScreen, Ui_AnimationChangingScreen>();
    #endif
}

/**
 * @brief Form initialization.
 * @param a_w Window GUI widget.
 * @param a_rotation Device display orientation.
 */
void DapUIAnimationScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation) {
    Q_UNUSED(a_rotation);

    // Initializing pointers to window widgets
    initChangedScreen(a_w);
}
