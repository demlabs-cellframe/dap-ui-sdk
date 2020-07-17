#include "BugReportLoadingScreen.h"

const QString BugReportLoadingScreen::SCREEN_NAME = "BugReportLoading";

BugReportLoadingScreen::BugReportLoadingScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
    , m_movie(":pics/Spin-1.2s-800px.gif")
{
    this->create(m_ui);


//    AdaptiveScreen::initScreen(this);
}

QString BugReportLoadingScreen::screenName()
{
    return BugReportLoadingScreen::SCREEN_NAME;
}

void BugReportLoadingScreen::initVariantUi(QWidget *a_widget)
{
#ifdef ANDROID
    m_ui->btnCancel->setText("CANCEL");
#else
    m_ui->layBugReportScreen->setAlignment(m_ui->frmBugReport, Qt::AlignHCenter);

    #if defined(Q_OS_MAC)
        m_ui->edtMessage->setAttribute(Qt::WA_MacShowFocusRect,false);
    #endif
#endif

    m_ui->lblGifLoading->setMovie(&m_movie);
    this->connectBtnToSignall(this->m_ui->btnCancel, &BugReportLoadingScreen::cancelled);
}

void BugReportLoadingScreen::showEvent(QShowEvent *a_event)
{
    this->m_movie.start();
    this->AdaptiveScreen::showEvent(a_event);
}

void BugReportLoadingScreen::hideEvent(QHideEvent *a_event)
{
    this->m_movie.stop();
    this->AdaptiveScreen::hideEvent(a_event);
}

void BugReportLoadingScreen::resizeEvent(QResizeEvent *a_event)
{
    this->m_movie.setScaledSize(this->m_ui->lblGifLoading->size());
    this->AdaptiveScreen::resizeEvent(a_event);
}
