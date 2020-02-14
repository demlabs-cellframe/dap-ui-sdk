#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QStateMachine>

#include "DapUiScreen.h"
#include "AdaptiveScreen.h"

#include "ui_StartScreen.h"
#include "CustomLineHeightLabel.h"

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

public slots:
    void setupConnectingState();
    void setupConnectedState();

protected:

    const QString BTN_SIGN_IN_NAME = "btnSignIn";
    const QString BTN_SIGN_UP_NAME = "btnSignUp";

    virtual void initVariantUi(QWidget *a_widget) override;

signals:

    void transitionTo_SignIn();
    void transitionTo_SignUp();

};

#endif // STARTSCREEN_H
