#include "SignInScreen.h"
#include "defines.h"

#include "ui_SignInScreen.h"
#include "StyledDropShadowEffect.h"

const QString SignInScreen::SCREEN_NAME = "SignIn";

SignInScreen::SignInScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
    , m_inputStates       (new QStateMachine(this))
    , m_stt_email         (new QState(m_inputStates))
    , m_stt_email_wrong   (new QState(m_stt_email))
    , m_stt_email_right   (new QState(m_stt_email))
    , m_stt_password      (new QState(m_inputStates))
    , m_stt_password_wrong(new QState(m_stt_password))
    , m_stt_password_right(new QState(m_stt_password))
{
    m_inputStates->setChildMode(QState::ChildMode::ParallelStates);
    m_stt_email   ->setInitialState(m_stt_email_right   );
    m_stt_password->setInitialState(m_stt_password_right);

    create<Ui::SignInScreen>();

    m_stt_email_right   ->addTransition(this, SIGNAL(wrongEmail    ()), m_stt_email_wrong   );
    m_stt_password_right->addTransition(this, SIGNAL(wrongPassword ()), m_stt_password_wrong);
    m_stt_email_wrong   ->addTransition(this, SIGNAL(emailEdited   (const QString &)), m_stt_email_right   );
    m_stt_password_wrong->addTransition(this, SIGNAL(passwordEdited(const QString &)), m_stt_password_right);


    connect(m_stt_email_right, &QState::entered, [this]{
        setValidationStateForEdit(this->EDT_EMAIL_NAME, this->LBL_EMAIL_ERROR);
    });
    connect(m_stt_email_wrong, &QState::entered, [this]{
        setValidationStateForEdit(this->EDT_EMAIL_NAME, this->LBL_EMAIL_ERROR, false);
    });
    connect(m_stt_password_right, &QState::entered, [this]{
        setValidationStateForEdit(this->EDT_PASSWORD_NAME, this->LBL_PASSWORD_ERROR);
    });
    connect(m_stt_password_wrong, &QState::entered, [this]{
        setValidationStateForEdit(this->EDT_PASSWORD_NAME, this->LBL_PASSWORD_ERROR, false);
    });

    connect(this, SIGNAL(emailEdited   (const QString &)), SLOT(setEmail   (const QString &)));
    connect(this, SIGNAL(passwordEdited(const QString &)), SLOT(setPassword(const QString &)));

    m_inputStates->start();
}


QString SignInScreen::screenName()
{
    return SCREEN_NAME;
}

void SignInScreen::setupInputState()
{
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Sign in");
}

void SignInScreen::setupConnectingState()
{
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Service connecting...");
}

void SignInScreen::setEmail(const QString &a_email)
{
    setChildProperties(EDT_EMAIL_NAME, "text", a_email);
    m_email = a_email;
}

void SignInScreen::setPassword(const QString &a_password)
{
    setChildProperties(EDT_PASSWORD_NAME, "text", a_password);
    m_password = a_password;
}

void SignInScreen::setErrorMessage(const QString &a_errorMsg)
{
    if (a_errorMsg == "Incorrect password")
    {
        setChildProperties(LBL_PASSWORD_ERROR, Properties::TEXT, a_errorMsg);
        setValidationStateForEdit(EDT_PASSWORD_NAME, LBL_PASSWORD_ERROR, false);

        emit wrongPassword();
    }
    else
    {
        setChildProperties(LBL_EMAIL_ERROR, Properties::TEXT, a_errorMsg);
        setValidationStateForEdit(EDT_EMAIL_NAME, LBL_EMAIL_ERROR, false);

        emit wrongEmail();
    }
};

void SignInScreen::checkFieldsAndSignIn()
{
    qDebug() << "checkFieldsAndSignIn";

    bool emailIsValid    = checkEmail();
    bool passwordIsValid = checkPassword();

    if (emailIsValid && passwordIsValid)
        emit this->signInRequest();
}

void SignInScreen::initVariantUi(QWidget *a_widget)
{
    QPushButton *btnSignIn        = a_widget->findChild<QPushButton*>(BTN_SIGN_IN_NAME  ); Q_ASSERT(btnSignIn);
    QLineEdit   *edtEmail         = a_widget->findChild<QLineEdit  *>(EDT_EMAIL_NAME    ); Q_ASSERT(edtEmail);
    QLineEdit   *edtPassword      = a_widget->findChild<QLineEdit  *>(EDT_PASSWORD_NAME ); Q_ASSERT(edtPassword);
    QLabel      *lblEmailError    = a_widget->findChild<QLabel     *>(LBL_EMAIL_ERROR   ); Q_ASSERT(lblEmailError);
    QLabel      *lblPasswordError = a_widget->findChild<QLabel     *>(LBL_PASSWORD_ERROR); Q_ASSERT(lblPasswordError);

    btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(btnSignIn));

    edtEmail->setPlaceholderText("Email");
    edtPassword->setPlaceholderText("Password");
    edtPassword->setEchoMode(QLineEdit::Password);
//    lblEmailError->setVisible(false);
//    lblPasswordError->setVisible(false);

//    connect(btnSignIn,&QPushButton::clicked,[=]{
//        lblEmailError->setVisible(true);
//        lblPasswordError->setVisible(true);
//    });

    connect(edtEmail   , SIGNAL(textEdited(const QString&)), this, SIGNAL(emailEdited   (const QString&)));
    connect(edtPassword, SIGNAL(textEdited(const QString&)), this, SIGNAL(passwordEdited(const QString&)));
    connect(btnSignIn  , SIGNAL(clicked())                 , this, SLOT(checkFieldsAndSignIn()));


//    edtEmail->setProperty(qPrintable(Properties::VALID), true);
//    edtEmail->style()->unpolish(lblEmailError);
//    edtEmail->style()->polish(lblEmailError);
//    edtEmail->update();

}

bool SignInScreen::checkPassword()
{
    if (m_password.isEmpty() || m_password.contains(' '))
    {
        emit this->wrongPassword();
        return false;
    }
    else
        return true;
}

bool SignInScreen::checkEmail()
{
    if (m_email.isEmpty() || m_email.contains(' '))
    {
        emit this->wrongEmail();
        return false;
    }
    else
        return true;
}

void SignInScreen::setValidationStateForEdit(const QString &a_editName, const QString &a_errorLabelName, bool a_valid /*= true*/)
{
    qDebug() << Properties::VALID << " = " << a_valid;
    this->setChildProperties(a_editName, Properties::VALID, a_valid);
    this->setChildProperties(a_errorLabelName, qPrintable(Properties::VISIBLE), !a_valid);

    updateChildStyle(a_editName);
}

