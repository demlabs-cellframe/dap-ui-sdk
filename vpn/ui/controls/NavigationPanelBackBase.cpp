#include "NavigationPanelBackBase.h"

const QString NavigationPanelBackBase::SCREEN_NAME = "NavigationPanelBackBase";

NavigationPanelBackBase::NavigationPanelBackBase(QWidget *parent)
    : AdaptiveScreen(parent)
{
    this->create(m_ui);

    AdaptiveScreen::initScreen(this);
}

QString NavigationPanelBackBase::screenName()
{
    return NavigationPanelBackBase::SCREEN_NAME;
}

void NavigationPanelBackBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

    this->connectBtnToSignall(m_ui->btnBack, &NavigationPanelBackBase::goBack);
}
