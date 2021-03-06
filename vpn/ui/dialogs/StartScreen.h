#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QStateMachine>
#include <QScopedPointer>
#include "AdaptiveScreen.h"
#include "ui_StartScreen.h"

#include "StyledDropShadowEffect.h"
#include "CustomLineHeightLabel.h"
#include "vpnDefine.h"



class StartScreen : public AdaptiveScreen
{
    Q_OBJECT

public:
    struct StateLinks;
    /// Overloaded constructor.
    /// @param a_parent Parent.
    StartScreen(QWidget * a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionState a_state);

protected:

    virtual void initVariantUi(QWidget *a_widget) override;

    QScopedPointer<Ui::StartScreen> m_ui;
};

#endif // STARTSCREEN_H
