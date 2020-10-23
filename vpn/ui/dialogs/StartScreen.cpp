#include "StartScreen.h"
#include "AppController.h"

const QString StartScreen::SCREEN_NAME = "Start";

/// Overloaded constructor.
/// @param a_parent Parent.
StartScreen::StartScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create(m_ui);

    AdaptiveScreen::initScreen(this);
}

QString StartScreen::screenName()
{
    return StartScreen::SCREEN_NAME;
}

void StartScreen::setState(ConnectionState a_state)
{
    QString statusText = (a_state == ConnectionState::ServersListLoading) ? tr("Loading servers list...") : tr("Connecting to service...");
    m_ui->lblSystemStatus->setText(statusText);
}

void StartScreen::setErrorMessage(const QString &msg)
{
    m_ui->lblSystemStatus->setText(msg);
}

void StartScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}

