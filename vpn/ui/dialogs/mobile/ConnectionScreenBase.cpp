#include "ConnectionScreenBase.h"

const QString ConnectionScreenBase::SCREEN_NAME = "ConnectionScreenBase";

ConnectionScreenBase::ConnectionScreenBase(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    //Add this to inheritable class if you want have working inharitance initVariantUi method:
    //this->create(m_ui);

    //And this:
    //AdaptiveScreen::initScreen(this);
}

QString ConnectionScreenBase::screenName()
{
    return ConnectionScreenBase::SCREEN_NAME;
}

void ConnectionScreenBase::setState(ConnectionState a_state)
{
    Q_UNUSED(a_state)
    qDebug() << "ConnectionScreenBase::setState " << this->statusText();

    if (a_state == m_state)
        return;
    m_state = a_state;

    m_ui->lblStatusMessage->setText(this->statusText());

    qDebug() << "ConnectionScreenBase::setState " << this->statusText();

    Utils::setPropertyAndUpdateStyle(m_ui->btnDisconnect, Properties::CONNECTED, a_state == ConnectionState::Connected);
}

QString ConnectionScreenBase::statusText()
{
    switch (m_state)
    {
    case ConnectionState::Disconnected:
        return tr("Disconnected");
    case ConnectionState::Connecting:
        return tr("Connecting...");
    case ConnectionState::Connected:
        return tr("Connected to %1").arg(m_currentServer);
    case ConnectionState::Disconnecting:
        return tr("Disconnecting...");
    case ConnectionState::ServerChanging:
        return tr("Changing server...");
    default:
        return QString();
    }
}

void ConnectionScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}
