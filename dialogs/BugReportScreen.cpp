#include "BugReportScreen.h"

const QString BugReportScreen::SCREEN_NAME = "BugReport";

BugReportScreen::BugReportScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::BugReportScreen>();

    connect(this, SIGNAL(emailEdited    (const QString &)), SLOT(setEmail   (const QString &)));
    connect(this, SIGNAL(reportEdited   (const QString &)), SLOT(setReport  (const QString &)));
}

QString BugReportScreen::screenName()
{
    return BugReportScreen::SCREEN_NAME;
}

void BugReportScreen::setEmail(const QString &a_email)
{
    setChildProperties(EDT_EMAIL, "text", a_email);
    m_email = a_email;
}

void BugReportScreen::setReport(const QString &a_report)
{
    setChildProperties(EDT_MESSAGE, "text", a_report);
    m_report_message = a_report;
}


void BugReportScreen::initVariantUi(QWidget *a_widget)
{
    CustomLineHeightLabel   *lblStatusMessage   = a_widget->findChild<CustomLineHeightLabel*>(LBL_STATUS_MESSAGE);  Q_ASSERT(lblStatusMessage);
    QPushButton             *btnSend            = a_widget->findChild<QPushButton*>(BTN_SEND);                      Q_ASSERT(btnSend);
    QLineEdit               *edtEmail           = a_widget->findChild<QLineEdit*>(EDT_EMAIL);                       Q_ASSERT(edtEmail);
    CustomLineHeightTextEdit *edtMessage        = a_widget->findChild<CustomLineHeightTextEdit*>(EDT_MESSAGE);      Q_ASSERT(edtMessage);

#ifdef Q_OS_ANDROID

    QLabel                  *lblCaption         = a_widget->findChild<QLabel*>(LBL_CAPTION);                        Q_ASSERT(lblCaption);
    lblCaption->setAlignment(Qt::AlignJustify);

    //Change color after editing
    connect(edtEmail,&QLineEdit::editingFinished,[=]{
        setChildProperties("edtEmail","state","endEdit");
        updateChildStyle("edtEmail");
    });

    edtEmail->setPlaceholderText("e-mail");
#else
    QLayout                 *vltBugReport = a_widget->findChild<QLayout*>(VLT_BUG_REPORT);                          Q_ASSERT(vltBugReport);
    btnSend->setGraphicsEffect(new StyledDropShadowEffect(btnSend));
    edtEmail->setPlaceholderText("Your email");
    vltBugReport->setAlignment(edtEmail,Qt::AlignHCenter);
    vltBugReport->setAlignment(edtMessage,Qt::AlignHCenter);
    vltBugReport->setAlignment(btnSend,Qt::AlignHCenter);
#endif
    lblStatusMessage->setVisible(false); 

    edtMessage->setPlaceholderText("Please describe the details of problem you faced. What actions did you take and what happened");

    connect(this, &BugReportScreen::wrongEmail, [=](){
            Utils::setPropertyAndUpdateStyle(edtEmail, Properties::ERROR, true);
        });
    connect(this, &BugReportScreen::wrongReport, [=](){
            Utils::setPropertyAndUpdateStyle(edtMessage, Properties::ERROR, true);
        });

    connect(edtEmail, &QLineEdit::textChanged, [=](){
            Utils::setPropertyAndUpdateStyle(edtEmail, Properties::ERROR, false);
        });
    connect(edtMessage, &CustomLineHeightTextEdit::textChanged, [=](){
            emit this->reportEdited(edtMessage->toPlainText());
            Utils::setPropertyAndUpdateStyle(edtMessage, Properties::ERROR, false);
        });
    connect(this, &BugReportScreen::sendReportRequest, [=](){
        edtEmail->setVisible(false);
        edtMessage->setVisible(false);
        lblStatusMessage->setVisible(true);
        btnSend->setText("Back");
    });
    connect(this, &BugReportScreen::goBack, [=](){
        edtEmail->clear();
        setEmail("");
        edtEmail->setVisible(true);
        edtMessage->clear();
        edtMessage->setPlaceholderText("Please describe the details of problem you faced. What actions did you take and what happened");
        edtMessage->setVisible(true);
        lblStatusMessage->setVisible(false);
        btnSend->setText("Send");
    });


    connect(btnSend, &QPushButton::clicked,[=](){
        if (btnSend->text() == "Send")
            checkFieldsAndSendReport();
        else if (btnSend->text() == "Back")
            emit this->goBack();
    });

    connect(edtEmail,   SIGNAL(textEdited(QString)),    this, SIGNAL(emailEdited(QString))  );

}

bool BugReportScreen::checkEmail()
{
    qDebug() << m_email;
    if (m_email.isEmpty() || m_email.contains(' ') || !m_email.contains('@'))
    {
        emit this->wrongEmail();
        return false;
    }
    else
        return true;
}

bool BugReportScreen::checkReport()
{
    if (m_report_message.isEmpty())
    {
        emit this->wrongReport();
        return false;
    }
    else
        return true;
}

void BugReportScreen::checkFieldsAndSendReport()
{

    bool emailIsValid   = checkEmail();
    bool reportIsValid  = checkReport();

    qDebug() << "checkFieldsAndSendReport" << emailIsValid << reportIsValid;
    if (emailIsValid && reportIsValid)
        emit this->sendReportRequest();
}

