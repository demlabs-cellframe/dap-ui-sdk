#include "AdaptiveScreen.h"

AdaptiveScreen::InitializerFunc AdaptiveScreen::s_initializerFunc = nullptr;

AdaptiveScreen::AdaptiveScreen(QWidget *a_parent /*= nullptr*/)
    : AdaptiveWidget(a_parent)
{
    AdaptiveScreen::initScreen(this);
}
