#include "BugReportWithEmailScreen.h"

BugReportScreenBase::BugReportScreenBase(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    //Add this to inheritable class if you want have working inharitance initVariantUi method:
    //this->create(m_ui);

    //And this:
    //AdaptiveScreen::initScreen(this);
}

void BugReportScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget);

#ifdef ANDROID
    m_ui->btnSend->setText(tr("SEND REPORT"));
    QScroller::grabGesture(m_ui->edtMessage->viewport(), QScroller::LeftMouseButtonGesture);
#else
    m_ui->layBugReportScreen->setAlignment(m_ui->frmBugReport, Qt::AlignHCenter);

    #if defined(Q_OS_MAC)
        m_ui->edtMessage->setAttribute(Qt::WA_MacShowFocusRect,false);
    #endif
#endif

    connect(this, &BugReportScreenBase::wrongMessage, [=](){
        Utils::setPropertyAndUpdateStyle(m_ui->edtMessage, Properties::WRONG, true);
    });

    connect(this->m_ui->edtMessage, &CustomTextEdit::textChanged, [=](){
        QString str = m_ui->edtMessage->toPlainText();
        QRegExp rx("\\\\");
        if (str.contains(rx)){
            QTextCursor fileViewerCursor = m_ui->edtMessage->textCursor();
            m_ui->edtMessage->setText(str.remove(rx));
            fileViewerCursor.movePosition(QTextCursor::End);
            m_ui->edtMessage->setTextCursor(fileViewerCursor);
        }

        emit this->messageChanged(str);
        Utils::setPropertyAndUpdateStyle(m_ui->edtMessage, Properties::WRONG, false);
    });

    connectBtnToSignall(m_ui->btnSend, &BugReportScreenBase::checkFieldsAndSendReport);
}

void BugReportScreenBase::setMessage(const QString &a_message)
{
    m_ui->edtMessage->setText(a_message);
}

bool BugReportScreenBase::checkFields()
{
    return (this->checkMessage());
}

void BugReportScreenBase::checkFieldsAndSendReport()
{
    if (this->checkFields())
        emit this->sendRequested();
}

bool BugReportScreenBase::checkMessage()
{
    if (m_ui->edtMessage->toPlainText().isEmpty())
    {
        emit this->wrongMessage();
        return false;
    }
    else
        return true;
}
