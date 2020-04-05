#include "SignUpScreen.h"



const QString SignUpScreen::SCREEN_NAME = "SignUp";

SignUpScreen::SignUpScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::SignUpScreen>();

    initScreen(this);
}

QString SignUpScreen::screenName()
{
    return SignUpScreen::SCREEN_NAME;
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
    QCheckBox               *chbInfo                = a_widget->findChild<QCheckBox*>(CHB_INFO);                        Q_ASSERT(chbInfo);
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
        Utils::setPropertyAndUpdateStyle(wgtAndroidError,Properties::STATE,Properties::ERROR);

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


    connect(btnSignUp,&QPushButton::clicked,[=]{

        lblEmailError->setVisible(true);
        lblPasswordError->setVisible(true);
        lblRptPasswordError->setVisible(true);

        Utils::setPropertyAndUpdateStyle(edtEmail,Properties::STATE,Properties::ERROR);
        Utils::setPropertyAndUpdateStyle(wgtEmailBottom,Properties::STATE,Properties::ERROR);


        Utils::setPropertyAndUpdateStyle(edtPassword,Properties::STATE,Properties::ERROR);
        Utils::setPropertyAndUpdateStyle(wgtFirstPasswordBottom,Properties::STATE,Properties::ERROR);

        Utils::setPropertyAndUpdateStyle(edtRptPassword,Properties::STATE,Properties::ERROR);
        Utils::setPropertyAndUpdateStyle(wgtSecondPasswordBottom,Properties::STATE,Properties::ERROR);
    });

#endif

}
