#ifndef ACCOUNTSCREEN_H
#define ACCOUNTSCREEN_H

#include <QLabel>

#include "StyledDropShadowEffect.h"

#include <QStateMachine>
#include <QListView>

#include "AdaptiveScreen.h"
#include "vpnDefine.h"

#include "defines.h"
#include <QStyle>
#include <QList>
#include "ui_AccountScreen.h"
#include "CustomPlacementButton.h"

class AccountScreen : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    AccountScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionStates a_state);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;
    const QString BTN_MONTH_PRICE      = "btnMonthPrice";
    const QString BTN_SIX_MONTH_PRICE  = "btnSixMonthPrice";
    const QString BTN_YEAR_PRICE       = "btnYearPrice";
};

#endif // ACCOUNTSCREEN_H
