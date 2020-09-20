#include "BugReportResultScreen.h"

const QString BugReportResultScreen::SCREEN_NAME = "BugReportResult";

BugReportResultScreen::BugReportResultScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create(m_ui);

    AdaptiveScreen::initScreen(this);
}

QString BugReportResultScreen::screenName()
{
    return BugReportResultScreen::SCREEN_NAME;
}

void BugReportResultScreen::setStatusMessage(const QString &a_message)
{
    this->m_ui->lblStatusMessage->setText(a_message);
}

void BugReportResultScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
#ifdef TEXT_VERSION_DIFFERENT
    m_ui->lblVersion->setText(QString("Version %1 %2").arg(DAP_VERSION).arg(__DATE__));
#endif

#ifdef ANDROID
    this->m_ui->btnClose->setText(tr("BACK"));
#else
    m_ui->layBugReportScreen->setAlignment(m_ui->frmBugReport, Qt::AlignHCenter);
#endif

    this->connectBtnToSignall(this->m_ui->btnClose, &BugReportResultScreen::goBack);
}
