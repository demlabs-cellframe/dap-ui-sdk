#include "SettingsScreen.h"

const QString SettingsScreen::SCREEN_NAME = "SettingsScreen";

SettingsScreen::SettingsScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::SettingsScreen>();
}

void SettingsScreen::initVariantUi(QWidget *a_widget)
{
    CustomComboBox          *cbbLanguage    = a_widget->findChild<CustomComboBox*>(CBB_LANGUAGE);          Q_ASSERT(cbbLanguage);

#ifdef Q_OS_ANDROID

    CustomPlacementButton   *btnLogOut      = a_widget->findChild<CustomPlacementButton*>(BTN_LOG_OUT);     Q_ASSERT(btnLogOut);
    CustomPlacementButton   *btnRenew       = a_widget->findChild<CustomPlacementButton*>(BTN_RENEW);      Q_ASSERT(btnRenew);

    //btnRenew->spacerActivate(ButtonSpaicer::Left);
    //btnLogOut->spacerActivate(ButtonSpaicer::Left);

    cbbLanguage->addItem("Language");
#else
    CustomComboBox *cbbBalance      = a_widget->findChild<CustomComboBox*>(CBB_BALABCE);       Q_ASSERT(cbbBalance);
    CustomComboBox *cbbBandwidth    = a_widget->findChild<CustomComboBox*>(CBB_BANDWIDTH);     Q_ASSERT(cbbBandwidth);
    CustomComboBox *cbbEncryption   = a_widget->findChild<CustomComboBox*>(CBB_ENCRYPTION);    Q_ASSERT(cbbEncryption);
    CustomComboBox *cbbUptime       = a_widget->findChild<CustomComboBox*>(CBB_UPTIME);        Q_ASSERT(cbbUptime);



    cbbBandwidth->addItem("Bandwidth");
    cbbUptime->addItem("Uptime");
    cbbEncryption->addItem("Encryption");
    cbbBalance->addItem("Balance");
    cbbLanguage->addItem("Language");
#endif
}
QString SettingsScreen::screenName()
{
    return SettingsScreen::SCREEN_NAME;
}
