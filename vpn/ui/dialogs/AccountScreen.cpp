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
    QPushButton   *btnMonthPrice     = a_widget->findChild<QPushButton*>(BTN_MONTH_PRICE);      Q_ASSERT(btnMonthPrice);
    QPushButton   *btnSixMonthPrice  = a_widget->findChild<QPushButton*>(BTN_SIX_MONTH_PRICE);  Q_ASSERT(btnSixMonthPrice);
    QPushButton   *btnYearPrice      = a_widget->findChild<QPushButton*>(BTN_YEAR_PRICE);       Q_ASSERT(btnYearPrice);

    connect(btnMonthPrice,&QPushButton::clicked, this,&AccountScreen::goToMonthPrice);
    connect(btnSixMonthPrice,&QPushButton::clicked, this,&AccountScreen::goToSixMonthPrice);
    connect(btnYearPrice,&QPushButton::clicked, this,&AccountScreen::goToYearPrice);

}
