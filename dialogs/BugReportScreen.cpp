#include "BugReportScreen.h"

const QString BugReportScreen::SCREEN_NAME = "BugReport";

BugReportScreen::BugReportScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::BugReportScreen>();
}

QString BugReportScreen::screenName()
{
    return BugReportScreen::SCREEN_NAME;
}

void BugReportScreen::setState(ConnectionStates a_state)
{
    Q_UNUSED(a_state)
}

void BugReportScreen::initVariantUi(QWidget *a_widget)
{
    CustomLineHeightLabel   *lblStatusMessage   = a_widget->findChild<CustomLineHeightLabel*>(LBL_STATUS_MESSAGE);  Q_ASSERT(lblStatusMessage);
    QPushButton             *btnSend            = a_widget->findChild<QPushButton*>(BTN_SEND);                      Q_ASSERT(btnSend);
    QLineEdit               *edtEmail           = a_widget->findChild<QLineEdit*>(EDT_EMAIL);                       Q_ASSERT(edtEmail);
    CustomLineHeightTextEdit *edtMessage        = a_widget->findChild<CustomLineHeightTextEdit*>(EDT_MESSAGE);      Q_ASSERT(edtMessage);

#ifdef Q_OS_ANDROID

    QLabel                  *lblCapture         = a_widget->findChild<QLabel*>(LBL_CAPTURE);                        Q_ASSERT(lblCapture);
    lblCapture->setAlignment(Qt::AlignJustify);

    //Change color after editing
    connect(edtEmail,&QLineEdit::editingFinished,[=]{
       edtEmail->setProperty("state","endEdit");
       edtEmail->style()->unpolish(edtEmail);
       edtEmail->style()->polish(edtEmail);
       edtEmail->update();
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

    connect(btnSend,&QPushButton::clicked,[=]{
        edtEmail->setVisible(false);
        edtMessage->setVisible(false);
        lblStatusMessage->setVisible(true);
        btnSend->setText("Back");
    });
}
