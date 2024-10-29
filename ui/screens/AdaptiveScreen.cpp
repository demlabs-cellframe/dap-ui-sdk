#include "AdaptiveScreen.h"

QMap<QString, void*> AdaptiveScreen::s_initFuncMap;

AdaptiveScreen::AdaptiveScreen(QWidget *a_parent /*= nullptr*/)
    : AdaptiveWidget(a_parent)
{
//    AdaptiveScreen::initScreen(this);
}
