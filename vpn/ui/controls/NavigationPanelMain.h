#ifndef NAVIGATIONPANELMAIN_H
#define NAVIGATIONPANELMAIN_H

#include "AdaptiveScreen.h"
#include <QLabel>

#include "ui_NavigationPanelMain.h"
#include "Utilz.h"
#include "defines.h"

class NavigationPanelMain : public AdaptiveScreen
{
    Q_OBJECT
#ifndef Q_OS_ANDROID
    QPushButton *m_btnBack;
    QPushButton *m_btnLogOut;
    QPushButton *m_btnAccount;
    QPushButton *m_btnConnections;
#endif
    QPushButton *m_btnSettings;
    QPushButton *m_btnFAQ;
    QPushButton *m_btnBug;
    QLabel *m_lblLeftLicense;

    void setVisibleButton(QPushButton *a_button,bool a_visible = true);
    void setCheckeButton(QPushButton *a_button);
public:
    NavigationPanelMain(QWidget *parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    virtual void initVariantUi(QWidget *a_widget) override;

#ifndef ANDROID

public slots:
    void setBackState(bool a_backState);
    bool backState();

    void setActiveScreen(const QString &a_nameScreen);

#endif

signals:
    void goToSettings();
    void goToBugReport();
    void goToFAQ();

#ifndef ANDROID
    void goBack();
    void goToAccount();
    void goToConnections();
#endif


protected:
    const QString BTN_BACK  = "btnBack";

private:
    const QString BTN_SETTINGS = "btnSettings";
    const QString BTN_HELP     = "btnHelp";
    const QString BTN_BUG      = "btnBug";
    const QString BTN_LEFTLICENSE = "lblLeftLicense";
#ifndef ANDROID
    const QString BTN_ACCOUNT    = "btnAccount";
    const QString BTN_CONNECTION = "btnConnection";
    const QString BTN_LOGOUT = "btnLogOut";

    bool m_backState = true;
#endif
};

#endif // NAVIGATIONPANELMAIN_H
