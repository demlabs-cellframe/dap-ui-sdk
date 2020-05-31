#include "StartScreen.h"
#include "AppController.h"

const QString StartScreen::SCREEN_NAME = "Start";

/// Overloaded constructor.
/// @param a_parent Parent.
StartScreen::StartScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::StartScreen>();
}

QString StartScreen::screenName()
{
    return StartScreen::SCREEN_NAME;
}

void StartScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}
