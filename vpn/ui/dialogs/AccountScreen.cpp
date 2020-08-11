#include "AccountScreen.h"

const QString AccountScreen::SCREEN_NAME = "Account";

AccountScreen::AccountScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    this->create(m_ui);
    AdaptiveScreen::initScreen(this);
}

QString AccountScreen::screenName()
{
    return AccountScreen::SCREEN_NAME;
}

void AccountScreen::setState(ConnectionState a_state)
{
    Q_UNUSED(a_state)
}

void AccountScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

#ifdef Q_OS_ANDROID
    m_ui->lblCaption->setText("Renew subscription");
#else
    m_ui->verticalLayout_4->setAlignment(m_ui->btnYearPrice,Qt::AlignHCenter);
    m_ui->verticalLayout_4->setAlignment(m_ui->btnMonthPrice,Qt::AlignHCenter);
    m_ui->verticalLayout_4->setAlignment(m_ui->btnSixMonthPrice,Qt::AlignHCenter);
#endif
}
