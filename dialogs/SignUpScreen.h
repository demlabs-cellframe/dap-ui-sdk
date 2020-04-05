#ifndef SIGNUPSCREEN_H
#define SIGNUPSCREEN_H

#include <QCheckBox>
#include "AdaptiveScreen.h"
#include "StyledDropShadowEffect.h"
#include "ui_SignUpScreen.h"
#include "CustomLineEdit.h"
#include "PasswordLineEdit.h"
#include "Utils.h"
#include "ClickableLabel.h"

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

    const QString BTN_SIGN_UP                   = "btnSignUp";
    const QString LBL_TERMS                     = "lblTerms";
    const QString LBL_PRIVACY_POLISY            = "lblPrivacyPolisy";
    const QString LBL_SIGN_IN                   = "lblSignIn";
    const QString EDT_EMAIL                     = "edtEmail";
    const QString EDT_PASSWORD                  = "edtPassword";
    const QString EDT_RPT_PASSWORD              = "edtRptPassword";
    const QString CHB_INFO                      = "chbInfo";
    const QString LBL_INFO                      = "lblInfo";
    const QString LBL_RPT_PASSWORD_ERROR        = "lblRptPasswordError";
    const QString LBL_PASSWORD_ERROR            = "lblPasswordError";
    const QString LBL_EMAIL_ERROR               = "lblEmailError";
    const QString WGT_INFO                      = "wgtInfo";
    const QString LBL_CAPTION                   = "lblCaption";
    const QString LBL_LOGO                      = "lblLogo";
    const QString LBL_ANDROID_ERROR             = "lblAndroidError";
    const QString WGT_ANDROID_ERROR             = "wgtAndroidError";
    const QString WGT_EMAIL_BOTTOM              = "wgtEmailBottom";
    const QString WGT_FIRST_PASSWORD_BOTTOM     = "wgtFirstPasswordBottom";
    const QString WGT_SECOND_PASSWORD_BOTTOM    = "wgtSecondPasswordBottom";
    const QString WGT_LEFT_MARGIN               = "wgtLeftMargin";
};
#endif // SIGNUPSCREEN_H
