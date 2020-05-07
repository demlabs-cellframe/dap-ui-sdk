#ifndef FORGETPASSWORDSCREEN_H
#define FORGETPASSWORDSCREEN_H

#include "AdaptiveScreen.h"
#include "QTextEdit"
#include "DapUiScreen.h"
#include "StyledDropShadowEffect.h"
#include "CustomLineHeightLabel.h"
#include <QVBoxLayout>
#include <QPushButton>

#include "ui_PasswordRecoveryScreen.h"
#include <QStyle>
#include "Utilz.h"

class PasswordRecoveryScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    PasswordRecoveryScreen(QWidget * a_parent);

    virtual QString screenName() override;
    static const QString SCREEN_NAME;

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
   // virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) override;

        virtual void initVariantUi(QWidget *a_widget) override;
private:
    const QString LBL_LOGO                  = "lblLogo";
    const QString BTN_SEND_MAIL             = "btnSendMail";
    const QString LBL_STATUS_MESSAGE        = "lblStatusMessage";
    const QString EDT_EMAIL                 = "edtEmail";
    const QString LBL_EMAIL_ERROR           = "lblEmailError";
    const QString VLT_MAIN_LAYOUT           = "vltMainLayout";
    const QString WGT_MARGIN_BOTTOM_MESSAGE = "wgtMarginBottomMessage";
    const QString LBL_CAPTION               = "lblCaption";
};
#endif // FORGETPASSWORDSCREEN_H
