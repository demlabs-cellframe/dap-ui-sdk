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

    QVBoxLayout             *vltMainSignUp          = a_widget->findChild<QVBoxLayout*>(VLT_MAIN_SIGN_UP);              Q_ASSERT(vltMainSignUp);
    QPushButton             *btnSignUp              = a_widget->findChild<QPushButton*>(BTN_SIGN_UP);                   Q_ASSERT(btnSignUp);
    QPushButton             *btnTerms               = a_widget->findChild<QPushButton*>(BTN_TERMS);                     Q_ASSERT(btnTerms);
    QPushButton             *btnPrivacyPolisy       = a_widget->findChild<QPushButton*>(BTN_PRIVACY_POLISY);            Q_ASSERT(btnPrivacyPolisy);
    QPushButton             *btnSignIn              = a_widget->findChild<QPushButton*>(BTN_SIGN_IN);                   Q_ASSERT(btnSignIn);
    CustomLineEdit          *edtEmail               = a_widget->findChild<CustomLineEdit*>(EDT_EMAIL);                  Q_ASSERT(edtEmail);
    CustomPasswordLineEdit  *edtPassword            = a_widget->findChild<CustomPasswordLineEdit*>(EDT_PASSWORD);       Q_ASSERT(edtPassword);
    CustomPasswordLineEdit  *edtRptPassword         = a_widget->findChild<CustomPasswordLineEdit*>(EDT_RPT_PASSWORD);   Q_ASSERT(edtRptPassword);
    QCheckBox               *chbInfo                = a_widget->findChild<QCheckBox*>(CHB_INFO);                        Q_ASSERT(chbInfo);
    QLabel                  *lblInfo                = a_widget->findChild<QLabel*>(LBL_INFO);                           Q_ASSERT(lblInfo);
    QLabel                  *lblRptPasswordError    = a_widget->findChild<QLabel*>(LBL_RPT_PASSWORD_ERROR);             Q_ASSERT(lblRptPasswordError);
    QLabel                  *lblPasswordError       = a_widget->findChild<QLabel*>(LBL_PASSWORD_ERROR);                 Q_ASSERT(lblPasswordError);
    QLabel                  *lblEmailError          = a_widget->findChild<QLabel*>(LBL_EMAIL_ERROR);                    Q_ASSERT(lblEmailError);
    QWidget                 *wgtInfo                = a_widget->findChild<QWidget*>(WGT_INFO);                          Q_ASSERT(wgtInfo);
    QLabel                  *lblCaption             = a_widget->findChild<QLabel*>(LBL_CAPTION);                        Q_ASSERT(lblCaption);
    QLabel                  *lblLogo                = a_widget->findChild<QLabel*>(LBL_LOGO);                           Q_ASSERT(lblLogo);

    QLabel                  *lblAndroidError        = a_widget->findChild<QLabel*>(LBL_ANDROID_ERROR);                  Q_ASSERT(lblAndroidError);
    QWidget                 *wgtAndroidError        = a_widget->findChild<QWidget*>(WGT_ANDROID_ERROR);                 Q_ASSERT(wgtAndroidError);

    QWidget                 *wgtEmailBottom         = a_widget->findChild<QWidget*>(WGT_EMAIL_BOTTOM);                  Q_ASSERT(wgtEmailBottom);
    QWidget                 *wgtFirstPasswordBottom = a_widget->findChild<QWidget*>(WGT_FIRST_PASSWORD_BOTTOM);         Q_ASSERT(wgtFirstPasswordBottom);
    QWidget                 *wgtSecondPasswordBottom= a_widget->findChild<QWidget*>(WGT_SECOND_PASSWORD_BOTTOM);        Q_ASSERT(wgtSecondPasswordBottom);

    lblAndroidError->setVisible(false);

    lblEmailError->setVisible(false);
    lblPasswordError->setVisible(false);
    lblRptPasswordError->setVisible(false);

#ifdef Q_OS_ANDROID
    QWidget                 *wgtLeftMargin          = a_widget->findChild<QWidget*>(WGT_LEFT_MARGIN);                    Q_ASSERT(wgtLeftMargin);

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
    btnSignUp->setGraphicsEffect(new StyledDropShadowEffect(btnSignUp));
    vltMainSignUp->setAlignment(lblEmailError,Qt::AlignCenter);
    vltMainSignUp->setAlignment(lblPasswordError,Qt::AlignCenter);
    vltMainSignUp->setAlignment(lblRptPasswordError,Qt::AlignCenter);
    vltMainSignUp->setAlignment(edtEmail,Qt::AlignCenter);
    vltMainSignUp->setAlignment(edtPassword,Qt::AlignCenter);
    vltMainSignUp->setAlignment(edtRptPassword,Qt::AlignCenter);
    vltMainSignUp->setAlignment(wgtInfo,Qt::AlignCenter);
    vltMainSignUp->setAlignment(btnSignUp,Qt::AlignCenter);
    vltMainSignUp->setAlignment(lblCaption,Qt::AlignCenter);
    vltMainSignUp->setAlignment(lblLogo,Qt::AlignCenter);
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
