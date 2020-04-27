#include "SignUpScreen.h"



const QString SignUpScreen::SCREEN_NAME = "SignUp";

SignUpScreen::SignUpScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::SignUpScreen>();

    initScreen(this);

    connect(this, SIGNAL(emailEdited        (const QString &)), SLOT(setEmail       (const QString &)));
    connect(this, SIGNAL(passwordEdited     (const QString &)), SLOT(setPassword    (const QString &)));
    connect(this, SIGNAL(repeatPasswordEdited(const QString &)), SLOT(setRptPassword(const QString &)));
    connect(this, SIGNAL(agreeChanged       (const bool &)),    SLOT(setAgree       (const bool &))    );


}

QString SignUpScreen::screenName()
{
    return SignUpScreen::SCREEN_NAME;
}

void SignUpScreen::setEmail(const QString &a_email)
{
    setChildProperties(EDT_EMAIL, "text", a_email);
    m_email = a_email;
}

void SignUpScreen::setPassword(const QString &a_password)
{
    setChildProperties(EDT_PASSWORD, "text", a_password);
    m_password = a_password;
}

void SignUpScreen::setRptPassword(const QString &a_rpt_password)
{
    setChildProperties(EDT_RPT_PASSWORD, "text", a_rpt_password);
    m_repeat_password = a_rpt_password;
}

void SignUpScreen::setAgree(const bool &a_agree)
{
    setChildProperties(CHB_AGREE, "bool", a_agree);
    m_agree = a_agree;
}

void SignUpScreen::initVariantUi(QWidget *a_widget)
{
    QPushButton             *btnSignUp              = a_widget->findChild<QPushButton*>(BTN_SIGN_UP);                   Q_ASSERT(btnSignUp);
    ClickableLabel             *lblTerms               = a_widget->findChild<ClickableLabel*>(LBL_TERMS);                     Q_ASSERT(lblTerms);

    ClickableLabel             *lblPrivacyPolisy       = a_widget->findChild<ClickableLabel*>(LBL_PRIVACY_POLISY);            Q_ASSERT(lblPrivacyPolisy);
    ClickableLabel             *lblSignIn              = a_widget->findChild<ClickableLabel*>(LBL_SIGN_IN);                   Q_ASSERT(lblSignIn);
    CustomLineEdit          *edtEmail               = a_widget->findChild<CustomLineEdit*>(EDT_EMAIL);                  Q_ASSERT(edtEmail);
    PasswordLineEdit        *edtPassword            = a_widget->findChild<PasswordLineEdit*>(EDT_PASSWORD);             Q_ASSERT(edtPassword);
    PasswordLineEdit        *edtRptPassword         = a_widget->findChild<PasswordLineEdit*>(EDT_RPT_PASSWORD);         Q_ASSERT(edtRptPassword);
    QCheckBox               *chbAgree                = a_widget->findChild<QCheckBox*>(CHB_AGREE);                      Q_ASSERT(chbAgree);
    QLabel                  *lblInfo                = a_widget->findChild<QLabel*>(LBL_INFO);                           Q_ASSERT(lblInfo);

    QWidget                 *wgtInfo                = a_widget->findChild<QWidget*>(WGT_INFO);                          Q_ASSERT(wgtInfo);
    QLabel                  *lblCaption             = a_widget->findChild<QLabel*>(LBL_CAPTION);                        Q_ASSERT(lblCaption);
    QLabel                  *lblLogo                = a_widget->findChild<QLabel*>(LBL_LOGO);                           Q_ASSERT(lblLogo);

    QWidget                 *wgtEmailBottom         = a_widget->findChild<QWidget*>(WGT_EMAIL_BOTTOM);                  Q_ASSERT(wgtEmailBottom);
    QWidget                 *wgtFirstPasswordBottom = a_widget->findChild<QWidget*>(WGT_FIRST_PASSWORD_BOTTOM);         Q_ASSERT(wgtFirstPasswordBottom);
    QWidget                 *wgtSecondPasswordBottom= a_widget->findChild<QWidget*>(WGT_SECOND_PASSWORD_BOTTOM);        Q_ASSERT(wgtSecondPasswordBottom);

#ifdef Q_OS_ANDROID
    QWidget                 *wgtLeftMargin          = a_widget->findChild<QWidget*>(WGT_LEFT_MARGIN);                    Q_ASSERT(wgtLeftMargin);
    QLabel                  *lblAndroidError        = a_widget->findChild<QLabel*>(LBL_ANDROID_ERROR);                  Q_ASSERT(lblAndroidError);
    QWidget                 *wgtAndroidError        = a_widget->findChild<QWidget*>(WGT_ANDROID_ERROR);                 Q_ASSERT(wgtAndroidError);
    lblAndroidError->setVisible(false);

    connect(btnSignUp,&QPushButton::clicked,[=]{
        lblAndroidError->setVisible(true);
        Utils::setPropertyAndUpdateStyle(wgtAndroidError,Properties::STATE,Properties::WRONG);

    });

    edtEmail->setAlignment(Qt::AlignLeft);
    edtPassword->setAlignment(Qt::AlignLeft);
    edtRptPassword->setAlignment(Qt::AlignLeft);

    wgtLeftMargin->setVisible(false);


    edtEmail->setPlaceholderText("e-mail");
    edtPassword->setPlaceholderText("password");
    edtPassword->setEchoMode(QLineEdit::Password);
    edtRptPassword->setPlaceholderText("repeat password");
    edtRptPassword->setEchoMode(QLineEdit::Password);

#else
    QLabel                  *lblRptPasswordError    = a_widget->findChild<QLabel*>(LBL_RPT_PASSWORD_ERROR);             Q_ASSERT(lblRptPasswordError);
    QLabel                  *lblPasswordError       = a_widget->findChild<QLabel*>(LBL_PASSWORD_ERROR);                 Q_ASSERT(lblPasswordError);
    QLabel                  *lblEmailError          = a_widget->findChild<QLabel*>(LBL_EMAIL_ERROR);                    Q_ASSERT(lblEmailError);

    lblEmailError->setVisible(false);
    lblPasswordError->setVisible(false);
    lblRptPasswordError->setVisible(false);

    btnSignUp->setGraphicsEffect(new StyledDropShadowEffect(btnSignUp));

    edtEmail->setPlaceholderText("Email");
    edtPassword->setPlaceholderText("Password");
    edtPassword->setEchoMode(QLineEdit::Password);
    edtRptPassword->setPlaceholderText("Repeat password");
    edtRptPassword->setEchoMode(QLineEdit::Password);
    //chbAgree->setChecked(false);

    connect(this, &SignUpScreen::wrongEmail, [=](){
        lblEmailError->setVisible(true);
        Utils::setPropertyAndUpdateStyle(edtEmail, Properties::WRONG, true);
        Utils::setPropertyAndUpdateStyle(wgtEmailBottom, Properties::WRONG, true);
    });
    connect(this, &SignUpScreen::wrongPassword, [=](){
        lblPasswordError->setVisible(true);
        Utils::setPropertyAndUpdateStyle(edtPassword, Properties::WRONG, true);
        Utils::setPropertyAndUpdateStyle(wgtFirstPasswordBottom, Properties::WRONG, true);
    });
    connect(this, &SignUpScreen::wrongRepeatPassword, [=](){
        lblRptPasswordError->setVisible(true);
        Utils::setPropertyAndUpdateStyle(edtRptPassword, Properties::WRONG, true);
        Utils::setPropertyAndUpdateStyle(wgtSecondPasswordBottom, Properties::WRONG, true);
    });

    connect(this, &SignUpScreen::agreeChanged, [=](){
        Utils::setPropertyAndUpdateStyle(chbAgree, Properties::WRONG, false);
    });
    connect(this, &SignUpScreen::notAgree, [=](){
        Utils::setPropertyAndUpdateStyle(chbAgree, Properties::WRONG, true);
    });

    connect(edtEmail, &CustomLineEdit::textChanged, [=](){
        lblEmailError->setVisible(false);
        Utils::setPropertyAndUpdateStyle(edtEmail, Properties::WRONG, false);
        Utils::setPropertyAndUpdateStyle(wgtEmailBottom, Properties::WRONG, false);
    });
    connect(edtPassword, &CustomLineEdit::textChanged, [=](){
        lblPasswordError->setVisible(false);
        Utils::setPropertyAndUpdateStyle(edtPassword, Properties::WRONG, false);
        Utils::setPropertyAndUpdateStyle(wgtFirstPasswordBottom, Properties::WRONG, false);
        lblRptPasswordError->setVisible(false);
        Utils::setPropertyAndUpdateStyle(edtRptPassword, Properties::WRONG, false);
        Utils::setPropertyAndUpdateStyle(wgtSecondPasswordBottom, Properties::WRONG, false);
    });
    connect(edtRptPassword, &CustomLineEdit::textChanged, [=](){
        lblPasswordError->setVisible(false);
        Utils::setPropertyAndUpdateStyle(edtPassword, Properties::WRONG, false);
        Utils::setPropertyAndUpdateStyle(wgtFirstPasswordBottom, Properties::WRONG, false);
        lblRptPasswordError->setVisible(false);
        Utils::setPropertyAndUpdateStyle(edtRptPassword, Properties::WRONG, false);
        Utils::setPropertyAndUpdateStyle(wgtSecondPasswordBottom, Properties::WRONG, false);
    });


#endif

    connect(lblSignIn,          SIGNAL(clicked()),              this, SIGNAL(goToSignIn())         );
    connect(lblTerms,           SIGNAL(clicked()),              this, SIGNAL(transitionToTerms())           );
    connect(lblPrivacyPolisy,   SIGNAL(clicked()),              this, SIGNAL(transitionToPrivacyPolicy())   );

    connect(edtEmail,           SIGNAL(textEdited(QString)),    this, SIGNAL(emailEdited(QString))          );
    connect(edtPassword,        SIGNAL(textEdited(QString)),    this, SIGNAL(passwordEdited(QString))       );
    connect(edtRptPassword,     SIGNAL(textEdited(QString)),    this, SIGNAL(repeatPasswordEdited(QString)) );
    connect(chbAgree,           SIGNAL(clicked(bool)),          this, SIGNAL(agreeChanged(bool))            );

    connect(btnSignUp,          SIGNAL(clicked(bool)),          this, SLOT(checkFieldsAndSignUp())          );



}

bool SignUpScreen::checkEmail()
{
    if (m_email.isEmpty() || m_email.contains(' ') || !m_email.contains('@'))
    {
        emit this->wrongEmail();
        return false;
    }
    else
        return true;
}

bool SignUpScreen::checkPassword()
{
    if (m_password.isEmpty() || m_password.contains(' '))
    {
        emit this->wrongPassword();
        return false;
    }
    else
        return true;
}

bool SignUpScreen::checkRepeatPassword()
{
    if (m_repeat_password != m_password || m_repeat_password.isEmpty())
    {
        emit this->wrongRepeatPassword();
        return false;
    }
    else
        return true;
}

bool SignUpScreen::checkAgree()
{
    if (!m_agree)
    {
        emit this->notAgree();
        return false;
    }
    else
        return true;
}

void SignUpScreen::checkFieldsAndSignUp()
{
    qDebug() << "checkFieldsAndSignUp";

    bool emailIsValid    = checkEmail();
    bool passwordIsValid = checkPassword();
    bool repeatPasswordIsValid = checkRepeatPassword();
    bool agreeIsValid = checkAgree();

    if (emailIsValid && passwordIsValid && repeatPasswordIsValid && agreeIsValid)
        emit this->signUpRequest();
}
