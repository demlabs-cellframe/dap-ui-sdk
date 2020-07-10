#include "SignUpScreen.h"
#include "QRegularExpressionValidator"


const QString SignUpScreen::SCREEN_NAME = "SignUp";

SignUpScreen::SignUpScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::SignUpScreen>();

    connect(this, SIGNAL(emailEdited            (const QString &)), SLOT(setEmail       (const QString &)));
    connect(this, SIGNAL(passwordEdited         (const QString &)), SLOT(setPassword    (const QString &)));
    connect(this, SIGNAL(repeatPasswordEdited   (const QString &)), SLOT(setRptPassword(const QString &)));
    connect(this, SIGNAL(agreeChanged           (const bool &)),    SLOT(setAgree       (const bool &))    );

    AdaptiveScreen::initScreen(this);
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
                            m_btnSignUp              = a_widget->findChild<QPushButton*>(BTN_SIGN_UP);                   Q_ASSERT(m_btnSignUp);
    ClickableLabel          *lblTerms               = a_widget->findChild<ClickableLabel*>(LBL_TERMS);                  Q_ASSERT(lblTerms);
    ClickableLabel          *lblSignIn              = a_widget->findChild<ClickableLabel*>(LBL_SIGN_IN);                Q_ASSERT(lblSignIn);
    ClickableLabel          *lblPrivacyPolisy       = a_widget->findChild<ClickableLabel*>(LBL_PRIVACY_POLISY);         Q_ASSERT(lblPrivacyPolisy);
                             m_edtEmail               = a_widget->findChild<CustomLineEdit*>(EDT_EMAIL);                Q_ASSERT(m_edtEmail);


                             m_edtPassword          = a_widget->findChild<PasswordLineEdit*>(EDT_PASSWORD);             Q_ASSERT(m_edtPassword);
                             m_edtRptPassword       = a_widget->findChild<PasswordLineEdit*>(EDT_RPT_PASSWORD);         Q_ASSERT(m_edtRptPassword);
    QPushButton             *btnAgree               = a_widget->findChild<QPushButton*>(CHB_AGREE);                     Q_ASSERT(btnAgree);
    QLabel                  *lblInfo                = a_widget->findChild<QLabel*>(LBL_INFO);                           Q_ASSERT(lblInfo);

    QWidget                 *wgtInfo                = a_widget->findChild<QWidget*>(WGT_INFO);                          Q_ASSERT(wgtInfo);
    QLabel                  *lblCaption             = a_widget->findChild<QLabel*>(LBL_CAPTION);                        Q_ASSERT(lblCaption);
    QLabel                  *lblStatusMessage                = a_widget->findChild<QLabel*>(LBL_STATUS_MESSAGE);        Q_ASSERT(lblStatusMessage);

    QWidget                 *wgtEmailBottom         = a_widget->findChild<QWidget*>(WGT_EMAIL_BOTTOM);                  Q_ASSERT(wgtEmailBottom);
    QWidget                 *wgtFirstPasswordBottom = a_widget->findChild<QWidget*>(WGT_FIRST_PASSWORD_BOTTOM);         Q_ASSERT(wgtFirstPasswordBottom);
    QWidget                 *wgtSecondPasswordBottom= a_widget->findChild<QWidget*>(WGT_SECOND_PASSWORD_BOTTOM);        Q_ASSERT(wgtSecondPasswordBottom);

    QLabel                  *lblThankYou            = a_widget->findChild<QLabel*>(LBL_THANK_YOU);                      Q_ASSERT(lblThankYou);
    QLabel                  *lblGoToChoosePlane     = a_widget->findChild<QLabel*>(LBL_GO_TO_CHOOSE_PLANE);             Q_ASSERT(lblGoToChoosePlane);
    QWidget                 *wgtMainFrame           = a_widget->findChild<QWidget*>(WGT_MAIN_FRAME);                    Q_ASSERT(wgtMainFrame);
    QWidget                 *wgtHAccount            = a_widget->findChild<QWidget*>(WGT_HACCOUNT);                      Q_ASSERT(wgtHAccount);


    lblThankYou->setVisible(false);
    lblGoToChoosePlane->setVisible(false);

    m_edtEmail->setValidator(new QRegularExpressionValidator(Utils::regularEmail(), this));

#ifdef Q_OS_MAC
    edtEmail->setAttribute(Qt::WA_MacShowFocusRect,false);
    m_edtPassword->setAttribute(Qt::WA_MacShowFocusRect,false);
    m_edtRptPassword->setAttribute(Qt::WA_MacShowFocusRect,false);
#endif

    m_edtEmail->setPlaceholderText("Email");
    m_edtPassword->setPlaceholderText("Password");
    m_edtPassword->setEchoMode(QLineEdit::Password);
    m_edtRptPassword->setPlaceholderText("Repeat password");
    m_edtRptPassword->setEchoMode(QLineEdit::Password);
 //   chbAgree->setChecked(false);


    connect(this, &SignUpScreen::wrongEmail, [=](){

        Utils::setPropertyAndUpdateStyle(m_edtEmail, Properties::WRONG, true);
        Utils::setPropertyAndUpdateStyle(lblStatusMessage, Properties::WRONG, true);
        lblStatusMessage->setText(tr("Not a valid email"));

    });

    connect(this, &SignUpScreen::wrongPassword, [=](){

        Utils::setPropertyAndUpdateStyle(m_edtPassword, Properties::WRONG, true);
        Utils::setPropertyAndUpdateStyle(lblStatusMessage, Properties::WRONG, true);
        lblStatusMessage->setText(tr("Choose a unique password"));
    });

    connect(this, &SignUpScreen::wrongCreatedUser, [=](const QString &a_message){

        Utils::setPropertyAndUpdateStyle(lblStatusMessage, Properties::WRONG, true);
        lblStatusMessage->setText(a_message);
        m_btnSignUp->setText(TEXT_SIGN_UP);
    });

    connect(this, &SignUpScreen::wrongRepeatPassword, [=](){

        Utils::setPropertyAndUpdateStyle(m_edtRptPassword, Properties::WRONG, true);
        Utils::setPropertyAndUpdateStyle(lblStatusMessage, Properties::WRONG, true);
        lblStatusMessage->setText(tr("Passwords must match"));
    });

    connect(this, &SignUpScreen::agreeChanged, [=](){
        Utils::setPropertyAndUpdateStyle(btnAgree, Properties::WRONG, false);
    });

    connect(this, &SignUpScreen::notAgree, [=](){
        Utils::setPropertyAndUpdateStyle(btnAgree, Properties::WRONG, true);
        lblStatusMessage->setText(QString(tr("I agree to the terms of %1")).arg(DAP_BRAND));
    });

    connect(m_edtEmail, &CustomLineEdit::textChanged,[=]{
        emit this->setStateStutusMessageNormal();
        Utils::setPropertyAndUpdateStyle(m_edtEmail, Properties::WRONG, false);
    });

    connect(m_edtPassword, &CustomLineEdit::textChanged,[=]{
        emit this->setStateStutusMessageNormal();
        Utils::setPropertyAndUpdateStyle(m_edtPassword, Properties::WRONG, false);
    });

    connect(m_edtRptPassword, &CustomLineEdit::textChanged,[=]{
        emit this->setStateStutusMessageNormal();
        Utils::setPropertyAndUpdateStyle(m_edtRptPassword, Properties::WRONG, false);
    });

    connect(this, &SignUpScreen::fieldEdited,[=]{
        emit this->setStateStutusMessageNormal();

        Utils::setPropertyAndUpdateStyle(m_edtEmail, Properties::WRONG, false);
        Utils::setPropertyAndUpdateStyle(m_edtPassword, Properties::WRONG, false);
        Utils::setPropertyAndUpdateStyle(m_edtRptPassword, Properties::WRONG, false);
    });

    connect(this,&SignUpScreen::setStateStutusMessageNormal,[=]{
        lblStatusMessage->setText(BrandProperties::DEFAULT_TEXT_STATUS_BAR_SIGNUPSCREEN);
        Utils::setPropertyAndUpdateStyle(lblStatusMessage, Properties::WRONG, false);
    });
    connect(this, &SignUpScreen::setStateNormalScreen,[=]{

            wgtMainFrame->setVisible(true);
            wgtHAccount->setVisible(true);
            lblThankYou->setVisible(false);
            lblGoToChoosePlane->setVisible(false);
            m_btnSignUp->setText(TEXT_CHOOSE_PLANE);

        emit this->fieldEdited();
    });

    connect(this, &SignUpScreen::setStateFinalScreen,[=]{

        wgtMainFrame->setVisible(false);
        wgtHAccount->setVisible(false);
        lblThankYou->setVisible(true);
        lblGoToChoosePlane->setVisible(true);
        m_btnSignUp->setText(TEXT_CHOOSE_PLANE);
    });

    connect(lblTerms,           SIGNAL(clicked()),              this, SIGNAL(transitionToTerms())           );
    connect(lblPrivacyPolisy,   SIGNAL(clicked()),              this, SIGNAL(transitionToPrivacyPolicy())   );

    connect(m_edtEmail,         SIGNAL(textEdited(QString)),    this, SIGNAL(emailEdited(QString))          );
    connect(m_edtPassword,      SIGNAL(textEdited(QString)),    this, SIGNAL(passwordEdited(QString))       );
    connect(m_edtRptPassword,   SIGNAL(textEdited(QString)),    this, SIGNAL(repeatPasswordEdited(QString)) );
    connect(btnAgree,           SIGNAL(clicked(bool)),          this, SIGNAL(agreeChanged(bool))            );
    connect(lblSignIn,          &ClickableLabel::clicked,       this, &SignUpScreen::goToSignIn);

    connect(m_mgr,              SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    connect(m_btnSignUp, &QPushButton::clicked,[=](){

        if(m_btnSignUp->text() == TEXT_SIGN_UP)
        {
            emit this->fieldEdited();
            checkFieldsAndSignUp();
        }

        if(m_btnSignUp->text() == TEXT_CHOOSE_PLANE)
        {
            QDesktopServices::openUrl(QUrl(BrandProperties::URL_CHOOSE_PRICE/*"https://kelvpn.com/pricing/"*/));
            emit this->goToSignIn();
            emit this->setStateNormalScreen();
        }
    });
}

bool SignUpScreen::checkEmail()
{
    if (!m_edtEmail->hasAcceptableInput())
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

void SignUpScreen::replyFinished(QNetworkReply *reply){

    this->setChildProperties(WGT_MAIN_FRAME, Properties::ENABLED , true);
    this->setChildProperties(BTN_SIGN_UP, Properties::ENABLED , true);

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray content = reply->readAll();
        qDebug() << content;
        QJsonDocument itemDoc = QJsonDocument::fromJson(content);
        QJsonObject itemObj = itemDoc.object();
        QVariantMap mainMap = itemObj.toVariantMap();
        QVariantMap map = mainMap["result"].toMap();
        QString message = map["message"].toString();
        //here you can place the response processing from the site
        if(message == "User created")
        {
            emit this->setStateFinalScreen();
            qDebug() << "User created";
        }
        else
        {
            QString MESS_USER_NAME_EXISTS(tr("Sorry, that username already exists!"));
            QString MESS_USER_CREATED_ERLIER(tr("Cannot create a user with an empty login name."));
            if(message == MESS_USER_NAME_EXISTS || message == MESS_USER_CREATED_ERLIER)
            {
                emit this->wrongCreatedUser(message);
            }
        }
    }
    else {
        qDebug() << "ERROR in POST: " + reply->errorString();
    }
        reply->deleteLater();
}

void SignUpScreen::checkFieldsAndSignUp()
{
    bool emailIsValid    = checkEmail();
    bool passwordIsValid = checkPassword();
    bool repeatPasswordIsValid = checkRepeatPassword();
    bool agreeIsValid = checkAgree();

    qDebug() << "checkFieldsAndSignUp";

    if (emailIsValid && passwordIsValid && repeatPasswordIsValid && agreeIsValid)
    {
        this->setChildProperties(WGT_MAIN_FRAME, Properties::ENABLED , false);
        this->setChildProperties(BTN_SIGN_UP, Properties::ENABLED , false);
        m_btnSignUp->setText(TEXT_CONNECTING);
        QNetworkRequest request;
        request.setUrl(QUrl(BrandProperties::URL_REGISTRATION/*"https://kelvpn.com/wp-json/dapvpn/v1/register/"*/));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        QString body = QString("email=%1&password=%2").arg(m_email).arg(m_password);
        m_mgr->post(request, body.toUtf8());
    }
}

