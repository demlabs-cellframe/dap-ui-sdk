#include "LoginScreenLayout.h"

#include "StyledDropShadowEffect.h"

const QString LoginScreenLayout::SCREEN_NAME = "LoginLayout";

LoginScreenLayout::LoginScreenLayout(QWidget *a_parent)
    :MultiScreenAbstract(a_parent)
{
    create<Ui::LoginScreenLayout>();

    AdaptiveScreen::initScreen(this);
}

QString LoginScreenLayout::screenName()
{
    return LoginScreenLayout::SCREEN_NAME;
}

LoginNavigationPanel *LoginScreenLayout::navigationPanel(QWidget *a_widget)
{
    if (!a_widget)
        a_widget = this->variant();

    LoginNavigationPanel *panel = a_widget->findChild<LoginNavigationPanel*>();
    Q_ASSERT(panel);

    return panel;
}


/**
* @brief Form initialization.
* @param a_w Window GUI widget.
*/
void LoginScreenLayout::initVariantUi(QWidget *a_widget)
{
    a_widget->setObjectName("LoginScreenLayout");
    this->initChangingWidget(a_widget);
}

