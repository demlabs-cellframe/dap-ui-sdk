#ifndef SIGNINSCREEN_H
#define SIGNINSCREEN_H

#include <QLineEdit>
#include <QLabel>
#include <QStateMachine>

#include "ScreenInfo.h"
#include "vpnDefine.h"
#include "defines.h"
#include "ui_SignInScreen.h"
#include "StyledDropShadowEffect.h"
#include "CustomLineEdit.h"
#include "PasswordLineEdit.h"
#include "ClickableLabel.h"
#include <QMainWindow>
#include "ComboBoxPopup.h"
#include <QStandardItemModel>

#ifdef ANDROID
    #include "AdaptiveScreen.h"
    typedef AdaptiveScreen ParentClass;
#else
    #include "ScreenWithComboBoxesAbstract.h"
    typedef ScreenWithComboBoxesAbstract ParentClass;
#endif

class SignInScreen : public ParentClass
{
    Q_OBJECT
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    SignInScreen(QWidget * a_parent);

    virtual QString screenName() override;
    static const QString SCREEN_NAME;

    void setState(ConnectionStates a_state);

public slots:

    void setEmail   (const QString &a_email   );
    void setPassword(const QString &a_password);

    void setErrorMessage(const QString &a_errorMsg);

signals:
    void serverChanged(int serverName);

    void emailEdited   (const QString &email   );
    void wrongEmail();

    void passwordEdited(const QString &password);
    void wrongPassword();

    void signInRequest();

    void goToSignUp();
protected:

    virtual void initVariantUi(QWidget *a_widget) override;

#ifndef ANDROID
    virtual QStringList comboBoxesNames() const override;
#endif
private:

    QAbstractItemModel *m_serversModel = nullptr;


    void setValidationStateForEdit(const QString& a_editName, const QString& a_errorLabelName, bool a_valid = true);
    QString buttonText(ConnectionStates a_connectionState);

    const QString EDT_EMAIL_NAME     = "edtEmail";
    const QString EDT_PASSWORD_NAME  = "edtPassword";
    const QString BTN_SIGN_IN_NAME   = "btnSignIn";
    const QString LBL_EMAIL_ERROR    = "lblEmailError";
    const QString LBL_PASSWORD_ERROR = "lblPasswordError";
    const QString BTN_SHOW_PASSWORD  = "btnShowPassword";
    const QString BTN_CLEAR_EMAIL    = "btnClearEmail";
    const QString CBB_SERVER         = "cbbServer";
    const QString LBL_SIGN_UP        = "lblSignUp";
    const QString LBL_FORGOT_PASS    = "lblForgotPass";


    QStateMachine *m_inputStates;
    QState *m_stt_email;
    QState *m_stt_email_wrong;
    QState *m_stt_email_right;
    QState *m_stt_password;
    QState *m_stt_password_wrong;
    QState *m_stt_password_right;

    QString m_email;
    QString m_password;
};


#endif // SIGNINSCREEN_H
