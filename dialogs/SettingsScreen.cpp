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

    //btnRenew->spacerActivate(ButtonSpaicer::Left);
    //btnLogOut->spacerActivate(ButtonSpaicer::Left);

    cbbLanguage->addItem("Language");
#else
    CustomButtonComboBox *cbbBalance      = a_widget->findChild<CustomButtonComboBox*>(CBB_BALABCE);       Q_ASSERT(cbbBalance);
    CustomButtonComboBox *cbbBandwidth    = a_widget->findChild<CustomButtonComboBox*>(CBB_BANDWIDTH);     Q_ASSERT(cbbBandwidth);
    CustomButtonComboBox *cbbEncryption   = a_widget->findChild<CustomButtonComboBox*>(CBB_ENCRYPTION);    Q_ASSERT(cbbEncryption);
    CustomButtonComboBox *cbbUptime       = a_widget->findChild<CustomButtonComboBox*>(CBB_UPTIME);        Q_ASSERT(cbbUptime);
    CustomButtonComboBox *cbbLanguage    = a_widget->findChild<CustomButtonComboBox*>(CBB_LANGUAGE);       Q_ASSERT(cbbLanguage);

    cbbBandwidth->setButtonControll(new CustomPlacementButton(ImagePos::Right));
    cbbUptime->setButtonControll(new CustomPlacementButton(ImagePos::Right));
    cbbEncryption->setButtonControll(new CustomPlacementButton(ImagePos::Right));
    cbbBalance->setButtonControll(new CustomPlacementButton(ImagePos::Right));
    cbbLanguage->setButtonControll(new CustomPlacementButton(ImagePos::Right));

    cbbBandwidth->setCaption("Bandwidth");
    cbbUptime->setCaption("Uptime");
    cbbEncryption->setCaption("Encryption");
    cbbBalance->setCaption("Balance");
    cbbLanguage->setCaption("Language");

    cbbBandwidth->setCaptionPolicy(CaptionPolicy::showAlways);
    cbbUptime->setCaptionPolicy(CaptionPolicy::showAlways);
    cbbEncryption->setCaptionPolicy(CaptionPolicy::showAlways);
    cbbBalance->setCaptionPolicy(CaptionPolicy::showAlways);
    cbbLanguage->setCaptionPolicy(CaptionPolicy::showAlways);

    cbbBandwidth->setGraphicsEffect(new StyledDropShadowEffect(cbbBandwidth));
    cbbUptime->setGraphicsEffect(new StyledDropShadowEffect(cbbUptime));
    cbbEncryption->setGraphicsEffect(new StyledDropShadowEffect(cbbEncryption));
    cbbBalance->setGraphicsEffect(new StyledDropShadowEffect(cbbBalance));
    cbbLanguage->setGraphicsEffect(new StyledDropShadowEffect(cbbLanguage));

#endif
}
QString SettingsScreen::screenName()
{
    return SettingsScreen::SCREEN_NAME;
}
