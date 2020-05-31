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

    QLabel* lblVersion       = a_widget->findChild<QLabel*>("lblVersion");          Q_ASSERT(lblVersion);

    lblVersion->setText(tr("Version %1 %2").arg(DAP_VERSION).arg(__DATE__));


}
