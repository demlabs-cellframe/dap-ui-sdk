#include "DapUIAnimationScreenAbstract.h"

/**
 * @brief Overloaded constructor.
 * @param a_parent Parent.
 * @param a_sw Application window stack.
 */
DapUIAnimationScreenAbstract::DapUIAnimationScreenAbstract(QObject *parent, QStackedWidget *a_sw)
    :DapUiScreen (parent, a_sw),
    m_wgChangedScreen(nullptr)
{
}

/**
 * @brief Activate child a_screen. a_screen must exist
 * @param a_screen pointer to child screen
 * @return pointer to this screen (parent)
 */
DapUIAnimationScreenAbstract *DapUIAnimationScreenAbstract::activateChildScreen(DapUiScreen *a_screen)
{
    Q_ASSERT_X(qobject_cast<DapUIAnimationScreenAbstract*>(a_screen->parent()) == this, "activateDescendantScreen", "screen is not a child of this screen");

    m_wgChangedScreen->setCurrentWidget(a_screen->sw());

    return (this);
}

/**
 * @brief Getter for m_wgChangedScreen
 * @return pointer to AnimationChangingWidget
 */
AnimationChangingWidget *DapUIAnimationScreenAbstract::wgChangedScreen()
{
    return m_wgChangedScreen;
}


/**
 * @brief Initialize m_wgChangedScreen. Must be called by initUi of inheritor class
 * @param a_widget pointer widget in which will be founded ChangedWidget
 */
void DapUIAnimationScreenAbstract::initChangedScreen(QWidget *a_widget)
{
    // initialize default a_widget
    if (!a_widget)
        a_widget = page(rotation());

    // Initializing pointers to window widgets
    m_wgChangedScreen = a_widget->findChild<AnimationChangingWidget*>();
    Q_ASSERT(m_wgChangedScreen);

    connect(m_wgChangedScreen, &AnimationChangingWidget::animationFinished, [=]{
        emit animationFinished();
    });

}

