#ifndef TERMSOFUSESCREEN_H
#define TERMSOFUSESCREEN_H

#include "AdaptiveScreen.h"
#include "vpnDefine.h"
#include "ui_TermsOfUseScreen.h"
#include <QScroller>

class TermsOfUseScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    TermsOfUseScreen(QWidget *a_parent);
    static const QString SCREEN_NAME;
    virtual QString screenName() override;


protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;
};

#endif // TERMSOFUSESCREEN_H
