#ifndef NAVIGATIONPANELMAIN_H
#define NAVIGATIONPANELMAIN_H

#include "AdaptiveScreen.h"


class NavigationPanelMain : public AdaptiveScreen
{
    Q_OBJECT
public:
    NavigationPanelMain(QWidget *parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    virtual void initVariantUi(QWidget *a_widget) override;

signals:
    void goToSettings();
    void goToBugReport();
    void goToFAQ();

private:
    const QString BTN_SETTINGS = "btnSettings";
    const QString BTN_HELP = "btnHelp";
    const QString BTN_BUG  = "btnBug";
};

#endif // NAVIGATIONPANELMAIN_H
