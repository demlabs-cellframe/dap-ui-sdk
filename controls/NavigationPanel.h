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

public slots:
    void setState(States a_state);

private:
    const QString BTN_LOGOUT_NAME = "btnLogOut";


};

#endif // NAVIGATIONPANEL_H
