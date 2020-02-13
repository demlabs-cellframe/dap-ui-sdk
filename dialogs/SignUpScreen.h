#ifndef SIGNUPSCREEN_H
#define SIGNUPSCREEN_H

#include <QCheckBox>
#include "AdaptiveScreen.h"
#include "StyledDropShadowEffect.h"

class SignUpScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    SignUpScreen(QWidget * a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

protected:

    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;

signals:
    void transitionTo_SignIn();


private:
    const QString BTN_SIGN_UP_NAME = "lblSignUp";
    const QString LBL_SIGN_IN_NAME = "lblSignIn";

};
#endif // SIGNUPSCREEN_H
