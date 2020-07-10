#include "PasswordRecoveryScreen.h"
#include "QRegularExpressionValidator"

const QString PasswordRecoveryScreen::SCREEN_NAME = "PasswordRecovery";

PasswordRecoveryScreen::PasswordRecoveryScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::PasswordRecoveryScreen>();

    AdaptiveScreen::initScreen(this);
}

QString PasswordRecoveryScreen::screenName()
{
    return SCREEN_NAME;
}

void PasswordRecoveryScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
                                m_btnSendMail = a_widget->findChild<QPushButton*>(BTN_SEND_MAIL);                               Q_ASSERT(m_btnSendMail);
                                m_lblStatusMessage = a_widget->findChild<CustomLineHeightLabel*>(LBL_STATUS_MESSAGE);           Q_ASSERT(m_lblStatusMessage);
                                m_edtEmail = a_widget->findChild<QLineEdit*>(EDT_EMAIL);                                        Q_ASSERT(m_edtEmail);
                                m_lblEmailError = a_widget->findChild<QLabel*>(LBL_EMAIL_ERROR);                                Q_ASSERT(m_lblEmailError);
        QLabel                  *lblCaption = a_widget->findChild<QLabel*>(LBL_CAPTION);                                        Q_ASSERT(lblCaption);
        QWidget                 *wgtEmailBottom = a_widget->findChild<QWidget*>(WGT_EMAIL_BOTTOM);                              Q_ASSERT(wgtEmailBottom);

        m_edtEmail->setValidator(new QRegularExpressionValidator(Utils::regularEmail(), this));
        lblCaption->setText(TEXT_FORGOT_PASSWORD);

        m_lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to<br>reset your password.");
        connect(this, &PasswordRecoveryScreen::wrongScreen, [=](const QString &a_message){
                Utils::setPropertyAndUpdateStyle(m_edtEmail, Properties::WRONG, true);

                m_lblEmailError->setText(a_message);
                m_btnSendMail->setEnabled(true);
                m_edtEmail->setEnabled(true);
                m_btnSendMail->setText(TEXT_SEND_EMAIL);
            });

        connect(m_edtEmail, &QLineEdit::textChanged, [=](){
                Utils::setPropertyAndUpdateStyle(m_edtEmail, Properties::WRONG, false);
                m_lblEmailError->clear();
            });

    connect(m_btnSendMail,&QPushButton::clicked,[=]{

        if (m_btnSendMail->text() == TEXT_SEND_EMAIL)
        {
            m_btnSendMail->setText(TEXT_CONNECTING);
            checkFieldsAndSendEmail();
        }
        else if (m_btnSendMail->text() == TEXT_BACK)
            emit this->goBack();
    });

    connect(this, &PasswordRecoveryScreen::emailSent,[=]{

                m_lblStatusMessage->setText(tr("Link to reset password has been sent successfully by e-mail."));
                Utils::setPropertyAndUpdateStyle(wgtEmailBottom, Properties::ACTIVE, true);
                lblCaption->setText(TEXT_CHECK_EMAIL);

                m_edtEmail->setVisible(false);
                m_lblEmailError->clear();
                m_btnSendMail->setText(TEXT_BACK);
    });


    connect(this, &PasswordRecoveryScreen::goBack,[=]{

        m_lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to reset your password.");
        Utils::setPropertyAndUpdateStyle(wgtEmailBottom, Properties::ACTIVE, false);
        m_edtEmail->setVisible(true);
        m_edtEmail->clear();
        m_btnSendMail->setText(TEXT_SEND_EMAIL);
        lblCaption->setText(TEXT_FORGOT_PASSWORD);
        emit this->goToSignIn();
    });
        connect(m_edtEmail,  &QLineEdit::textEdited, this, &PasswordRecoveryScreen::setEmail);
        connect(m_mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processReply(QNetworkReply*)));

}

void PasswordRecoveryScreen::processReply(QNetworkReply *reply){

    m_btnSendMail->setEnabled(true);
    m_edtEmail->setEnabled(true);

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray content = reply->readAll();
        qDebug() << content;
        QJsonDocument itemDoc = QJsonDocument::fromJson(content);
        QJsonObject itemObj = itemDoc.object();
        QVariantMap mainMap = itemObj.toVariantMap();
        QVariantMap map = mainMap["result"].toMap();
        QString message = map["message"].toString();
        QString code = map["code"].toString();

        if(code == "0")
        {//message << Link to reset password has been sent successfully by e-mail.
            emit this->emailSent();
        }
        else
        {//Error: There is no account with that email address
         //Error: Email could not be sent. Your site may not be configured correctly to send e-mail or the text of the e-mail is empty.
            if(message == "Error: There is no account with that email address")
                emit wrongScreen(tr("There is no account with that email address."));
            else if (message == "Error: Email could not be sent. Your site may not be configured correctly to send e-mail or the text of the e-mail is empty")
            {
                emit wrongScreen(tr("Email could not be sent."));
            }
        }
    }
    else {
        qDebug() << "ERROR in POST: " + reply->errorString();
    }
        reply->deleteLater();
}

bool PasswordRecoveryScreen::checkEmail()
{
    qDebug() << m_email;
    if (!m_edtEmail->hasAcceptableInput())
    {
        emit this->wrongScreen(tr("Input a valid email"));
        return false;
    }
    else
        return true;
}

void PasswordRecoveryScreen::setEmail(const QString &a_email)
{
    setChildProperties(EDT_EMAIL, "text", a_email);
    for(int i=0;i<a_email.size(); i++){
        if(a_email[i].isLower())
            a_email[i].toUpper();
        else
            a_email[i].toLower();
    }
    m_email = a_email;
}
void PasswordRecoveryScreen::checkFieldsAndSendEmail()
{
    if (checkEmail())
    {
        m_btnSendMail->setText(TEXT_CONNECTING);
        m_btnSendMail->setEnabled(false);
        m_edtEmail->setEnabled(false);
        QNetworkRequest request;
        request.setUrl(QUrl(BrandProperties::URL_RESET_PASSWORD/*"https://kelvpn.com/wp-json/dapvpn/v1/register/"*/));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        QString body = QString("email=%1").arg(m_email);
        m_mgr->post(request, body.toUtf8());


    }
}
