#ifndef NAVIGATIONPANEL_H
#define NAVIGATIONPANEL_H

#include "AdaptiveWidget.h"
#include "CustomPlacementButton.h"
#include "DefaultMultiScreen.h"

class NavigationPanel
#ifdef Q_OS_ANDROID
        : public DefaultMultiScreen
#else
        : public AdaptiveWidget
#endif
{
    Q_OBJECT
public:
    enum class States {Main, Back};

    explicit NavigationPanel(QWidget *parent = nullptr);

    virtual void initVariantUi(QWidget *a_widget) override;

signals:
    void logout();

    void goToBugReport();
    void goToFAQ();
    void goBack();
    void goToSettings();

public slots:
    void setState(States a_state);

private:

#ifndef Q_OS_ANDROID
    const QString BTN_LOGOUT_NAME = "btnLogOut";
    const QString BTN_BUG  = "btnBug";
    const QString BTN_HELP = "btnHelp";
#endif

};

#endif // NAVIGATIONPANEL_H
