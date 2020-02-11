#include "SettingsScreen.h"

SettingsScreen::SettingsScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::SettingsScreen>();
}

void SettingsScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_w)
    Q_UNUSED(a_rotation)

    CustomComboBox *cbbBalance = a_w->findChild<CustomComboBox*>("cbbBalance");
    CustomComboBox *cbbBandwidth = a_w->findChild<CustomComboBox*>("cbbBandwidth");
    CustomComboBox *cbbEncryption = a_w->findChild<CustomComboBox*>("cbbEncryption");
    CustomComboBox *cbbLanguage = a_w->findChild<CustomComboBox*>("cbbLanguage");
    CustomComboBox *cbbUptime = a_w->findChild<CustomComboBox*>("cbbUptime");

    Q_ASSERT(cbbBalance);
    Q_ASSERT(cbbBandwidth);
    Q_ASSERT(cbbEncryption);
    Q_ASSERT(cbbLanguage);
    Q_ASSERT(cbbUptime);

    cbbBandwidth->addItem("Bandwidth");
    cbbUptime->addItem("Uptime");
    cbbEncryption->addItem("Encryption");
    cbbBalance->addItem("Balance");
    cbbLanguage->addItem("Language");
}
