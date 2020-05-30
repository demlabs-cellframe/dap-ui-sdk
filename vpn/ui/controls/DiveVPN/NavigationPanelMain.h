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

    QPushButton *m_btnBack = nullptr;
    QPushButton *m_btnSettings = nullptr;
    QPushButton *m_btnBug= nullptr;
    QLabel *m_lblLeftLicense = nullptr;
    QLabel *m_lblLastScreen = nullptr;

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
    const QString LBL_NAME_LAST_SCREEN     = "lblLastScreen";
    const QString BTN_BUG      = "btnBug";
    const QString LBL_LEFTLICENSE = "lblLeftLicense";

    bool m_backState = true;

};

#endif // NAVIGATIONPANELMAIN_H
