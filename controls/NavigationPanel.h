#ifndef NAVIGATIONPANEL_H
#define NAVIGATIONPANEL_H

#include "AdaptiveWidget.h"
#include "CustomPlacementButton.h"

class NavigationPanel : public AdaptiveWidget
{
    Q_OBJECT
public:
    explicit NavigationPanel(QWidget *parent = nullptr);

    virtual void initVariantUi(QWidget *a_widget) override;

signals:
    void logout();

private:
    const QString BTN_LOGOUT_NAME = "btnLogOut";


};

#endif // NAVIGATIONPANEL_H
