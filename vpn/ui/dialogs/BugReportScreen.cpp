#include "BugReportScreen.h"

const QString BugReportScreen::SCREEN_NAME = "BugReport";

BugReportScreen::BugReportScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::BugReportScreen>();

    connect(this, SIGNAL(emailEdited    (const QString &)), SLOT(setEmail   (const QString &)));
    connect(this, SIGNAL(reportEdited   (const QString &)), SLOT(setReport  (const QString &)));

    AdaptiveScreen::initScreen(this);
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
    CustomTextEdit          *edtMessage         = a_widget->findChild<CustomTextEdit*>(EDT_MESSAGE);                Q_ASSERT(edtMessage);

#ifdef Q_OS_ANDROID

    QLabel                  *lblCaption         = a_widget->findChild<QLabel*>(LBL_CAPTION);                        Q_ASSERT(lblCaption);
    CustomLineEdit          *edtEmail           = a_widget->findChild<CustomLineEdit*>(EDT_EMAIL );                 Q_ASSERT(edtEmail);

    lblCaption->setAlignment(Qt::AlignJustify);

    edtEmail->setPlaceholderText("e-mail");

    QScroller::grabGesture(edtMessage->viewport(), QScroller::LeftMouseButtonGesture);

    btnSend->setText("SEND");
#else
    QLineEdit               *edtEmail           = a_widget->findChild<QLineEdit*>(EDT_EMAIL);                       Q_ASSERT(edtEmail);
    QLayout                 *vltBugReport = a_widget->findChild<QLayout*>(VLT_BUG_REPORT);                          Q_ASSERT(vltBugReport);


    btnSend->setGraphicsEffect(new StyledDropShadowEffect(btnSend));
    edtEmail->setPlaceholderText("Your email");
    vltBugReport->setAlignment(edtEmail,Qt::AlignHCenter);
    vltBugReport->setAlignment(edtMessage,Qt::AlignHCenter);
    vltBugReport->setAlignment(btnSend,Qt::AlignHCenter);
#endif

#ifdef Q_OS_MAC
    edtEmail->setAttribute(Qt::WA_MacShowFocusRect,false);
    edtMessage->setAttribute(Qt::WA_MacShowFocusRect,false);
#endif

    lblStatusMessage->setVisible(false); 

    edtMessage->setPlaceholderText("Please describe the details of problem you faced. What actions did you take and what happened");

    connect(this, &BugReportScreen::wrongEmail, [=](){
            Utils::setPropertyAndUpdateStyle(edtEmail, Properties::WRONG, true);
        });
    connect(this, &BugReportScreen::wrongReport, [=](){
            Utils::setPropertyAndUpdateStyle(edtMessage, Properties::WRONG, true);
        });

    connect(edtEmail, &QLineEdit::textChanged, [=](){
            Utils::setPropertyAndUpdateStyle(edtEmail, Properties::WRONG, false);
        });
    connect(edtMessage, &CustomTextEdit::textChanged, [=](){
            emit this->reportEdited(edtMessage->toPlainText());
            Utils::setPropertyAndUpdateStyle(edtMessage, Properties::WRONG, false);
        });
    connect(this, &BugReportScreen::sendReportRequest, [=](){
        edtEmail->setVisible(false);
        edtMessage->setVisible(false);
        lblStatusMessage->setVisible(true);
        lblStatusMessage->setText("Bug report sending...");
        //btnSend->setText("Cancel");
#ifndef Q_OS_ANDROID
        btnSend->setText("Cancel");
#else
        btnSend->setText("BACK");
#endif
    });

    connect(this, &BugReportScreen::bugReportSent, [=](){
        lblStatusMessage->setText("Bug report sent<br>successfully");
        btnSend->setText("Cancel");
    });
    connect(this, &BugReportScreen::bugReportSendingError, [=](){
        lblStatusMessage->setText("Bug report sending error :(");
        btnSend->setText("Cancel");
    });

    connect(this, &BugReportScreen::goBack, [=](){
        edtEmail->clear();
        setEmail("");
        edtEmail->setVisible(true);
        edtMessage->clear();
        edtMessage->setPlaceholderText("Please describe the details of problem you faced. What actions did you take and what happened");
        edtMessage->setVisible(true);
        lblStatusMessage->setVisible(false);
#ifndef Q_OS_ANDROID
        btnSend->setText("Send");
#else
        btnSend->setText("SEND");
#endif
    });


    connect(btnSend, &QPushButton::clicked,[=](){
        if (btnSend->text() == "Send")
            checkFieldsAndSendReport();
        else if (btnSend->text() == "Back" || btnSend->text() == "Cancel")
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
        emit this->sendReportRequest(m_email, m_report_message);
}

