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

};

#endif // NAVIGATIONPANELMAIN_H
