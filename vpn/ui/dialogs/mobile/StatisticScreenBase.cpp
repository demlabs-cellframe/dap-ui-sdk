#include "StatisticScreenBase.h"

const QString StatisticScreenBase::SCREEN_NAME = "StatisticScreenBase";

StatisticScreenBase::StatisticScreenBase(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    //Add this to inheritable class if you want have working inharitance initVariantUi method:
    //this->create(m_ui);

    //And this:
    //AdaptiveScreen::initScreen(this);
    create(m_ui);

    AdaptiveScreen::initScreen(this);
}

QString StatisticScreenBase::screenName()
{
    return StatisticScreenBase::SCREEN_NAME;
}

void StatisticScreenBase::setAuthorized(bool a_authorized)
{

}

void StatisticScreenBase::setStreamOpened(bool a_streamOpened)
{

}

void StatisticScreenBase::setVirtualNetwork(bool a_virtualNetwork)
{

}

void StatisticScreenBase::setSentReceivedIndicators(int a_bytesReceived, int a_bytesSent, int a_packetsReceived, int a_packetsSent)
{

}

void StatisticScreenBase::initVariantUi(QWidget *a_widget)
{


    Q_UNUSED(a_widget)
}
