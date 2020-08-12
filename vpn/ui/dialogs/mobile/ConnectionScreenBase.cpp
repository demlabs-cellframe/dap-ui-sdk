#include "ConnectionScreenBase.h"

const QString ConnectionScreenBase::SCREEN_NAME = "ConnectionScreenBase";

ConnectionScreenBase::ConnectionScreenBase(QWidget *a_parent)
    : ScreenWithScreenPopupsAbstract(a_parent)
{
    //Add this to inheritable class if you want have working inharitance initVariantUi method:
    //this->create(m_ui);

    //And this:
    //AdaptiveScreen::initScreen(this);

    connect(m_ui->btnDisconnect, &QPushButton::clicked, this, &ConnectionScreenBase::disconnectionRequested);
}

QString ConnectionScreenBase::screenName()
{
    return ConnectionScreenBase::SCREEN_NAME;
}

void ConnectionScreenBase::setState(ConnectionState a_state)
{
    qDebug() << "ConnectionScreenBase::setState" << a_state;

    Q_UNUSED(a_state)

    if (a_state == m_state)
        return;
    m_state = a_state;

    m_ui->lblStatusMessage->setText(this->statusText());

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

void ConnectionScreenBase::setCurrentServer(const QString &a_currentServer)
{
    qDebug() << "ConnectionScreenBase::setCurrentServer:" << a_currentServer;
    if (m_currentServer == a_currentServer)
        return;
    m_currentServer = a_currentServer;

    m_ui->lblStatusMessage->setText(this->statusText());
}

void ConnectionScreenBase::initVariantUi(QWidget *a_widget)
{
    this->ScreenWithScreenPopupsAbstract::initVariantUi(a_widget);
}


QList<CustomPopup *> ConnectionScreenBase::customPopups()
{
    return {m_ui->cbbServer->popup()};
}
