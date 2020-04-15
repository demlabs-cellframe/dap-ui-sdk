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
    CustomComboBox          *cbbLanguage    = a_widget->findChild<CustomComboBox*>(CBB_LANGUAGE);           Q_ASSERT(cbbLanguage);
    CustomPlacementButton   *btnLogOut      = a_widget->findChild<CustomPlacementButton*>(BTN_LOG_OUT);     Q_ASSERT(btnLogOut);
    CustomPlacementButton   *btnRenew       = a_widget->findChild<CustomPlacementButton*>(BTN_RENEW);       Q_ASSERT(btnRenew);

    cbbLanguage->addItem("Language");
#else
    ComboBox *cbbBalance      = a_widget->findChild<ComboBox*>(CBB_BALABCE);       Q_ASSERT(cbbBalance);
    ComboBox *cbbBandwidth    = a_widget->findChild<ComboBox*>(CBB_BANDWIDTH);     Q_ASSERT(cbbBandwidth);
    ComboBox *cbbEncryption   = a_widget->findChild<ComboBox*>(CBB_ENCRYPTION);    Q_ASSERT(cbbEncryption);
    ComboBox *cbbUptime       = a_widget->findChild<ComboBox*>(CBB_UPTIME);        Q_ASSERT(cbbUptime);
    ComboBox *cbbLanguage    = a_widget->findChild<ComboBox*>(CBB_LANGUAGE);       Q_ASSERT(cbbLanguage);

    cbbBandwidth ->buttonControll()->setImagePosition(ImagePos::Right);
    cbbUptime    ->buttonControll()->setImagePosition(ImagePos::Right);
    cbbEncryption->buttonControll()->setImagePosition(ImagePos::Right);
    cbbBalance   ->buttonControll()->setImagePosition(ImagePos::Right);
    cbbLanguage  ->buttonControll()->setImagePosition(ImagePos::Right);

    cbbBandwidth ->setCaption("Bandwidth");
    cbbUptime    ->setCaption("Uptime");
    cbbEncryption->setCaption("Encryption");
    cbbBalance   ->setCaption("Balance");
    cbbLanguage  ->setCaption("Language");

    cbbBandwidth ->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    cbbUptime    ->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    cbbEncryption->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    cbbBalance   ->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    cbbLanguage  ->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);

#endif
}
QString SettingsScreen::screenName()
{
    return SettingsScreen::SCREEN_NAME;
}
