#ifndef PRIVACYPOLICYSCREEN_H
#define PRIVACYPOLICYSCREEN_H
#include "AdaptiveScreen.h"
#include "vpnDefine.h"
#include "ui_PrivacyPolicyScreen.h"
#include <QScroller>

class PrivacyPolicyScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    PrivacyPolicyScreen(QWidget *a_parent);
    static const QString SCREEN_NAME;
    virtual QString screenName() override;

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;
};

#endif // PRIVACYPOLICYSCREEN_H
