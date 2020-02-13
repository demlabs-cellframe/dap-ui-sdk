#ifndef SIGNINSCREEN_H
#define SIGNINSCREEN_H

#include <QLineEdit>
#include <QLabel>

#include "AdaptiveScreen.h"
#include "ScreenInfo.h"


class SignInScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    SignInScreen(QWidget * a_parent);

    virtual QString screenName() override;
    static const QString SCREEN_NAME;

public slots:

    void setEmail   (const QString &a_email   );
    void setPassword(const QString &a_password);

signals:
    void emailEdited   (const QString &email   );
    void passwordEdited(const QString &password);
    void signInRequest();

protected:

    virtual void initVariantUi(QWidget *a_widget) override;

private:
    const QString EDT_EMAIL_NAME    = "edtEmail";
    const QString EDT_PASSWORD_NAME = "edtPassword";
    const QString BTN_SIGN_IN       = "btnSignIn";
};


#endif // SIGNINSCREEN_H
