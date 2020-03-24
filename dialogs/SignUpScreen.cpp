#include "SignUpScreen.h"

#include "ui_SignUpScreen.h"

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
    QPushButton *btnSignUp           = a_widget->findChild<QPushButton*>(BTN_SIGN_UP_NAME);
    QLabel      *lblSignIn           = a_widget->findChild<QLabel     *>(LBL_SIGN_IN_NAME);
    QLineEdit   *edtEmail            = a_widget->findChild<QLineEdit*>("edtEmail");
    QLineEdit   *edtPassword         = a_widget->findChild<QLineEdit*>("edtPassword");
    QLineEdit   *edtRptPassword      = a_widget->findChild<QLineEdit*>("edtRptPassword");
    QCheckBox   *cbInfo              = a_widget->findChild<QCheckBox*>("cbInfo");
    QLabel      *lblInfo             = a_widget->findChild<QLabel*>("lblInfo");
    QLabel      *lblRptPasswordError = a_widget->findChild<QLabel*>("lblRptPasswordError");
    QLabel      *lblPasswordError    = a_widget->findChild<QLabel*>("lblPasswordError");
    QLabel      *lblEmailError       = a_widget->findChild<QLabel*>("lblEmailError");

    Q_ASSERT(btnSignUp);
    Q_ASSERT(edtEmail);
    Q_ASSERT(edtPassword);
    Q_ASSERT(edtRptPassword);
    Q_ASSERT(cbInfo);
    Q_ASSERT(lblInfo);
    Q_ASSERT(lblRptPasswordError);
    Q_ASSERT(lblPasswordError);
    Q_ASSERT(lblEmailError);

    btnSignUp->setGraphicsEffect(new StyledDropShadowEffect(btnSignUp));

    edtEmail->setPlaceholderText("Email");
    edtPassword->setPlaceholderText("Password");
    edtPassword->setEchoMode(QLineEdit::Password);
    edtRptPassword->setPlaceholderText("Repet password");
    edtRptPassword->setEchoMode(QLineEdit::Password);


    lblInfo->setText("<p>I argee to the <span style='color:#D61F5D;'>terms</span> and <span style='color:#D61F5D;'>privacy policy</span></p>");

    QObject::connect(btnSignUp,&QPushButton::clicked,[=]{
        lblEmailError->setMaximumWidth(1000);
        lblPasswordError->setMaximumWidth(1000);
        lblRptPasswordError->setMaximumWidth(1000);
    });

//    QObject::connect(lblSignIn, &QLabel::clicked, [this]
//    {
//        qDebug()<<"clicked";
//        emit this->transitionTo_SignIn();
//    });
}
