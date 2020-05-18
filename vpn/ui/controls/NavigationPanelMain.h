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

#ifndef ANDROID
public slots:
    void setBackState(bool a_backState);
    bool backState();
#endif

signals:
    void goToSettings();
    void goToBugReport();
    void goToFAQ();

#ifndef ANDROID
    void goBack();
#endif


protected:
    const QString BTN_BACK  = "btnBack";

private:
    const QString BTN_SETTINGS = "btnSettings";
    const QString BTN_HELP     = "btnHelp";
    const QString BTN_BUG      = "btnBug";

#ifndef ANDROID
    const QString BTN_ACCOUNT    = "btnAccount";
    const QString BTN_CONNECTION = "btnConnection";

    bool m_backState = true;
#endif
};

#endif // NAVIGATIONPANELMAIN_H
