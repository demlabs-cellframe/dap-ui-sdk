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
    void goToSettings();
    void goToBugReport();

#ifndef ANDROID
public slots:
    void setBackState(bool a_backState);
    bool backState();
#endif

private:

    const QString BTN_BACK = "btnBack";

#ifndef ANDROID
    bool m_backState = true;
#endif
};

#endif // NAVIGATIONPANELBACK_H
