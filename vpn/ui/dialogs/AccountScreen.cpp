#include "AccountScreen.h"

const QString AccountScreen::SCREEN_NAME = "Account";

AccountScreen::AccountScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::AccountScreen>();
    AdaptiveScreen::initScreen(this);
}

QString AccountScreen::screenName()
{
    return AccountScreen::SCREEN_NAME;
}

void AccountScreen::setState(ConnectionStates a_state)
{
    Q_UNUSED(a_state)
}

void AccountScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

}
