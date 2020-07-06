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
    for(int i=0;i<a_email.size(); i++){
        if(a_email[i].isLower())
            a_email[i].toUpper();
        else
            a_email[i].toLower();
    }
    m_email = a_email;
}

void BugReportScreen::setReport(const QString &a_report)
{
    setChildProperties(EDT_MESSAGE, "text", a_report);
    for(int i=0;i<a_report.size(); i++){
        if(a_report[i].isLower())
            a_report[i].toUpper();
        else
            a_report[i].toLower();
    }
    m_report_message = a_report;
}


void BugReportScreen::initVariantUi(QWidget *a_widget)
{
    CustomLineHeightLabel   *lblStatusMessage   = a_widget->findChild<CustomLineHeightLabel*>(LBL_STATUS_MESSAGE);  Q_ASSERT(lblStatusMessage);
    QPushButton             *btnSend            = a_widget->findChild<QPushButton*>(BTN_SEND);                      Q_ASSERT(btnSend);
    CustomTextEdit          *edtMessage         = a_widget->findChild<CustomTextEdit*>(EDT_MESSAGE);                Q_ASSERT(edtMessage);

    QLabel                  *lblGifLoading      = a_widget->findChild<QLabel*>(LBL_GIF_LOADING);                    Q_ASSERT(lblGifLoading);
                            edtEmail           = a_widget->findChild<QLineEdit*>(EDT_EMAIL );                       Q_ASSERT(edtEmail);
#ifdef Q_OS_ANDROID

    QLabel                  *lblCaption         = a_widget->findChild<QLabel*>(LBL_CAPTION);                        Q_ASSERT(lblCaption);
    QWidget                 *wgtTopButton         = a_widget->findChild<QWidget*>("wgtTopButton");                        Q_ASSERT(wgtTopButton);
    QWidget                 *wgtBottomEmail         = a_widget->findChild<QWidget*>("wgtBottomEmail");                        Q_ASSERT(wgtBottomEmail);


    lblStatusMessage->setWordWrap(true);

    lblCaption->setAlignment(Qt::AlignJustify);

    edtEmail->setPlaceholderText("e-mail");

    QScroller::grabGesture(edtMessage->viewport(), QScroller::LeftMouseButtonGesture);

    btnSend->setText("SEND");
#else
    QLayout                 *vltBugReport       = a_widget->findChild<QLayout*>(VLT_BUG_REPORT);                    Q_ASSERT(vltBugReport);
    edtEmail->setPlaceholderText("Your email");

    btnSend->setGraphicsEffect(new StyledDropShadowEffect(btnSend));
    vltBugReport->setAlignment(edtEmail,Qt::AlignHCenter);
    vltBugReport->setAlignment(edtMessage,Qt::AlignHCenter);
    vltBugReport->setAlignment(btnSend,Qt::AlignHCenter);
#endif

#ifdef Q_OS_MAC
    edtEmail->setAttribute(Qt::WA_MacShowFocusRect,false);
    edtMessage->setAttribute(Qt::WA_MacShowFocusRect,false);
#endif
    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption) ;
    edtEmail->setValidator(new QRegularExpressionValidator(rx, this));
QMovie *movie = new QMovie(":pics/Spin-1.2s-800px.gif", nullptr, lblGifLoading);
    lblStatusMessage->setVisible(false); 
    lblGifLoading->setVisible(false);

    edtMessage->setPlaceholderText(tr("Please describe the problem you've encountered. What you have done and what happened"));

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
    connect(this, &BugReportScreen::sendingBugReportRequest, [=](){
        edtEmail->setVisible(false);
        edtMessage->setVisible(false);
        lblStatusMessage->setVisible(true);
        lblStatusMessage->setText("Sending...");
        movie->setScaledSize(QSize(100,100)); //need for dont pixiling
        lblGifLoading->setMovie(movie);
        lblGifLoading->setVisible(true);
        movie->start();
#ifndef Q_OS_ANDROID
        btnSend->setText(TEXT_CANCEL);
#else
        Utils::setPropertyAndUpdateStyle(lblStatusMessage,Properties::ACTIVE,true);
        btnSend->setVisible(false);
#endif
    });

    connect(this, &BugReportScreen::bugReportSent, [=](const QString &reply){

        lblStatusMessage->setText("Bug report sent<br>successfully. #" + reply);
        lblGifLoading->setMovie(nullptr);  //todo set gif

        btnSend->setText(TEXT_BACK);
#ifdef Q_OS_ANDROID
        btnSend->setVisible(true);
        Utils::setPropertyAndUpdateStyle(lblStatusMessage,Properties::ACTIVE,false);
        wgtBottomEmail->setVisible(false);
        wgtTopButton->setVisible(false);
#endif
    });
    connect(this, &BugReportScreen::bugReportSendingError, [=](){
        lblStatusMessage->setText("Bug report sending error.");
        lblGifLoading->setMovie(nullptr); //todo set gif
        btnSend->setText(TEXT_BACK);
#ifdef Q_OS_ANDROID
        btnSend->setVisible(true);
        Utils::setPropertyAndUpdateStyle(lblStatusMessage,Properties::ACTIVE,false);
        wgtBottomEmail->setVisible(false);
        wgtTopButton->setVisible(false);
#endif
    });

    connect(this, &BugReportScreen::goBack, [=](){
        edtEmail->clear();
        setEmail("");
        edtEmail->setVisible(true);
        edtMessage->clear();
        edtMessage->setPlaceholderText(tr("Please describe the problem you've encountered. What you have done and what happened"));
        edtMessage->setVisible(true);
        lblStatusMessage->setVisible(false);
        lblGifLoading->setVisible(false);

        btnSend->setText(TEXT_SEND);
#ifdef Q_OS_ANDROID
        wgtBottomEmail->setVisible(true);
        wgtTopButton->setVisible(true);
#endif
    });


    connect(btnSend, &QPushButton::clicked,[=](){
        if (btnSend->text() == TEXT_SEND)
            checkFieldsAndSendReport();
        else if (btnSend->text() == TEXT_BACK || btnSend->text() == TEXT_CANCEL)
            emit this->goBack();
    });

    connect(edtEmail,   SIGNAL(textEdited(QString)),    this, SIGNAL(emailEdited(QString))  );

}

bool BugReportScreen::checkEmail()
{
    qDebug() << m_email;

    if (!edtEmail->hasAcceptableInput())
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
        emit this->sendingBugReportRequest(m_email, m_report_message);
}
