#include "ConnectionScreenBase.h"

const QString ConnectionScreenBase::SCREEN_NAME = "ConnectionScreenBase";

ConnectionScreenBase::ConnectionScreenBase(QWidget *a_parent)
    : ScreenWithScreenPopupsAbstract(a_parent)
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

void ConnectionScreenBase::initVariantUi(QWidget *a_widget)
{
    connect(m_ui->btnDisconnect, &QPushButton::clicked, this, &ConnectionScreenBase::disconnectionRequested);
    connect(m_ui->cbbServer, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), [this](const QString &a_activatedServer){
        emit this->serverChangingRequested(a_activatedServer);
    });

    this->ScreenWithScreenPopupsAbstract::initVariantUi(a_widget);
}

void ConnectionScreenBase::setState(ConnectionState a_state)
{
    qDebug() << "ConnectionScreenBase::setState" << a_state;

    if (a_state == m_state)
        return;
    m_state = a_state;

    m_ui->lblStatusMessage->setText(this->statusText());

    bool isConnected(a_state == ConnectionState::Connected);
    Utils::setPropertyAndUpdateStyle(m_ui->btnDisconnect, Properties::CONNECTED, isConnected);

    m_ui->cbbServer->setEnabled(isConnected);
    m_ui->btnDisconnect->setEnabled(isConnected);
}

QString ConnectionScreenBase::currentServer()
{
    return m_ui->cbbServer->currentText();
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
        return getConnectedToText().arg(this->currentServer() == "Auto" ? tr("Auto") : this->currentServer());
    case ConnectionState::Disconnecting:
        return tr("Disconnecting...");
    case ConnectionState::ServerChanging:
        return tr("Changing server...");
    default:
        return QString();
    }
}
const QString ConnectionScreenBase::getConnectedToText()
{
    return QString(tr("Connected to %1"));

}
void ConnectionScreenBase::setCurrentServer(const QString &a_currentServer)
{
    if (this->currentServer() == a_currentServer)
        return;

    m_ui->cbbServer->setCurrentText(a_currentServer);
    m_ui->lblStatusMessage->setText(this->statusText());
}


QList<CustomPopup *> ConnectionScreenBase::customPopups()
{
    return {m_ui->cbbServer->popup()};
}
