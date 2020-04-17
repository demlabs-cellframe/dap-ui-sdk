#ifndef NAVIGATIONPANELBACK_H
#define NAVIGATIONPANELBACK_H

#include "AdaptiveScreen.h"

class NavigationPanelBack : public AdaptiveScreen
{
    Q_OBJECT
public:
    NavigationPanelBack(QWidget *parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    virtual void initVariantUi(QWidget *a_widget) override;

signals:
    void goBack();

private:

    const QString BTN_BACK = "btnBack";
};

#endif // NAVIGATIONPANELBACK_H
