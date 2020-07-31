#ifndef FORGETPASSWORDSCREEN_H
#define FORGETPASSWORDSCREEN_H

#include "AdaptiveScreen.h"
#include "QTextEdit"
#include "StyledDropShadowEffect.h"
#include "CustomLineHeightLabel.h"
#include <QVBoxLayout>
#include <QPushButton>

#include "ui_PasswordRecoveryScreen.h"
#include <QStyle>
#include "Utilz.h"
#include "BrandProperties.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QShortcut>

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

public slots:
    void setEmail   (const QString &a_email   );

    void processReply(QNetworkReply *reply);
    void passwordRecovery();
signals:
    void wrongScreen(const QString &a_message);

    void emailSent();
    void goBack();
    void goToSignIn();

    void setStateNormal();
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
    const QString WGT_EMAIL_BOTTOM          = "wgtEmailBottom";
    const QString LBL_CAPTION               = "lblCaption";

    const QString TEXT_FORGOT_PASSWORD      = tr("Forgot password?");
    const QString TEXT_CHECK_EMAIL          = tr("Check email");

#ifdef Q_OS_ANDROID
    const QString TEXT_CONNECTING           = tr("CONNECTING...");
    const QString TEXT_BACK                 = tr("BACK");
    const QString TEXT_SEND_EMAIL           = tr("SEND EMAIL");
#else
    const QString TEXT_CONNECTING           = tr("Connecting...");
    const QString TEXT_BACK                 = tr("Back");
    const QString TEXT_SEND_EMAIL           = tr("Send email");
#endif

    QLineEdit *m_edtEmail = nullptr;
    CustomLineHeightLabel   *m_lblStatusMessage = nullptr;
    QLabel                  *m_lblEmailError = nullptr;
    QPushButton             *m_btnSendMail = nullptr;

    QNetworkAccessManager *m_mgr = new QNetworkAccessManager(this);


    bool checkEmail();
    void checkFieldsAndSendEmail();
    QString m_email;
};
#endif // FORGETPASSWORDSCREEN_H
