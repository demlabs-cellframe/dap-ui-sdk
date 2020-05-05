#include "SignInScreen.h"

#include "DapDataLocal.h"


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

void SignInScreen::setState(ConnectionStates a_state)
{
    setChildProperties(BTN_SIGN_IN_NAME, Properties::TEXT   , buttonText(a_state));
    this->setProperty(qPrintable(Properties::ENABLED), a_state == ConnectionStates::Disconnected);
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
#ifndef Q_OS_ANDROID
        setChildProperties(LBL_PASSWORD_ERROR, Properties::TEXT, a_errorMsg);
        setValidationStateForEdit(EDT_PASSWORD_NAME, LBL_PASSWORD_ERROR, false);

        emit wrongPassword();
#endif
    }
    else
    {

        setChildProperties(LBL_EMAIL_ERROR, Properties::TEXT, a_errorMsg);
        setValidationStateForEdit(EDT_EMAIL_NAME, LBL_EMAIL_ERROR, false);

        emit wrongEmail();
    }

}

void SignInScreen::checkFieldsAndSignIn()
{
    if (checkEmail() && checkPassword())
        emit this->signInRequest();
}

void SignInScreen::initVariantUi(QWidget *a_widget)
{
    QPushButton    *btnSignIn      = a_widget->findChild<QPushButton*>(BTN_SIGN_IN_NAME  );      Q_ASSERT(btnSignIn);
    QLabel         *lblEmailError  = a_widget->findChild<QLabel     *>(LBL_EMAIL_ERROR   );      Q_ASSERT(lblEmailError);
    ClickableLabel *lblSignUp      = a_widget->findChild<ClickableLabel*>(LBL_SIGN_UP );         Q_ASSERT(lblSignUp);
    ComboBox       *cbbServer    ; Utils::findChild(a_widget, CBB_SERVER       , cbbServer  );
    QLineEdit      *edtEmail     ; Utils::findChild(a_widget, EDT_EMAIL_NAME   , edtEmail   );
    QLineEdit      *edtPassword  ; Utils::findChild(a_widget, EDT_PASSWORD_NAME, edtPassword);

    QLabel         *lblForgotPass; Utils::findChild(a_widget, LBL_FORGOT_PASS  , lblForgotPass);
    lblForgotPass->setText("");


    if (!m_serversModel)
    {
        m_serversModel = cbbServer->model();

        for (DapServerInfo& server :DapDataLocal::me()->servers())
            cbbServer->addItem(server.name);

        connect(DapDataLocal::me(), &DapDataLocal::serverAdded, [cbbServer](const DapServerInfo& a_serverInfo){
            cbbServer->addItem(a_serverInfo.name);
        });

        connect(DapDataLocal::me(), SIGNAL(serversCleared()), cbbServer, SLOT(clear()));
    }
    else
        cbbServer->setModel(m_serversModel);
    cbbServer->QComboBox::setCurrentText(DapDataLocal::me()->serverName());

    connect(cbbServer  , SIGNAL(activated(int))            , this, SIGNAL(serverChanged(int)));
    connect(edtEmail   , SIGNAL(textEdited(const QString&)), this, SIGNAL(emailEdited   (const QString&)));
    connect(edtPassword, SIGNAL(textEdited(const QString&)), this, SIGNAL(passwordEdited(const QString&)));
    connect(btnSignIn  , SIGNAL(clicked())                 , this, SLOT  (checkFieldsAndSignIn()));
    connect(lblSignUp  , SIGNAL(clicked())                 , this, SIGNAL(goToSignUp()) );
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
    this->setChildProperties(a_editName, qPrintable(Properties::WRONG), !a_valid);
    this->setChildProperties(a_errorLabelName, qPrintable(Properties::WRONG), !a_valid);

    updateChildStyle(a_editName);
    updateChildStyle(a_errorLabelName);
}

QString SignInScreen::buttonText(ConnectionStates a_connectionState)
{
    switch (a_connectionState)
    {
        case ConnectionStates::Connecting:
            return "Server connecting...";
        default:
            return "Sign in";
    }
}

