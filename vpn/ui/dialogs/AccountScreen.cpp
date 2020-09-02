#include "AccountScreen.h"

const QString AccountScreen::SCREEN_NAME = "Account";

AccountScreen::AccountScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
    #ifndef Q_OS_ANDROID
    , m_serialRemovalMessage(new SerialRemovalConfirmationMessage(this))
    #endif
{
    this->create(m_ui);
#ifndef Q_OS_ANDROID
    m_serialRemovalMessage->setWindowType(Qt::Dialog);
    m_serialRemovalMessage->setObjectName("ScreenMessagePopup");
    connect(m_serialRemovalMessage, &SerialRemovalConfirmationMessage::accepted, this, &AccountScreen::serialRemovalRequested);
#endif

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
    connect(m_ui->btnResetSerial, &QPushButton::clicked, m_serialRemovalMessage, &SerialRemovalConfirmationMessage::show);


//    m_ui->verticalLayout_4->setAlignment(m_ui->btnYearPrice,Qt::AlignHCenter);
//    m_ui->verticalLayout_4->setAlignment(m_ui->btnMonthPrice,Qt::AlignHCenter);
//    m_ui->verticalLayout_4->setAlignment(m_ui->btnSixMonthPrice,Qt::AlignHCenter);
#endif
}

#ifndef Q_OS_ANDROID
void AccountScreen::setState(ActivationState a_activationState)
{
    m_ui->btnResetSerial->setEnabled(a_activationState == ActivationState::Activated);
}
#endif
