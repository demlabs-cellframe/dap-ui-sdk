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
}

void ConnectionScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

}
