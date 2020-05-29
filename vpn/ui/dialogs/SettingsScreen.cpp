#include "SettingsScreen.h"

const QString SettingsScreen::SCREEN_NAME = "SettingsScreen";

SettingsScreen::SettingsScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::SettingsScreen>();
}

void SettingsScreen::initVariantUi(QWidget *a_widget)
{


#ifdef Q_OS_ANDROID
    CustomPopupComboBox     *cbbLanguage    = a_widget->findChild<CustomPopupComboBox*>(CBB_LANGUAGE);           Q_ASSERT(cbbLanguage);
    CustomPlacementButton   *btnLogOut      = a_widget->findChild<CustomPlacementButton*>(BTN_LOG_OUT);     Q_ASSERT(btnLogOut);
    CustomPlacementButton   *btnRenew       = a_widget->findChild<CustomPlacementButton*>(BTN_RENEW);       Q_ASSERT(btnRenew);

    cbbLanguage  ->setCaption("Language");
#else

    ComboBox *cbbEncryption   = a_widget->findChild<ComboBox*>(CBB_ENCRYPTION);    Q_ASSERT(cbbEncryption);
    ComboBox *cbbUptime       = a_widget->findChild<ComboBox*>(CBB_UPTIME);        Q_ASSERT(cbbUptime);
    ComboBox *cbbLanguage    = a_widget->findChild<ComboBox*>(CBB_LANGUAGE);       Q_ASSERT(cbbLanguage);


    cbbUptime    ->buttonControll()->setLayoutDirection(Qt::LeftToRight);
    cbbEncryption->buttonControll()->setLayoutDirection(Qt::LeftToRight);
    cbbLanguage  ->buttonControll()->setLayoutDirection(Qt::LeftToRight);


    cbbUptime    ->setCaption("Uptime");
    cbbEncryption->setCaption("Encryption");
    cbbLanguage  ->setCaption("Language");

    cbbUptime    ->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    cbbEncryption->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    cbbLanguage  ->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);

#endif
}
QString SettingsScreen::screenName()
{
    return SettingsScreen::SCREEN_NAME;
}
