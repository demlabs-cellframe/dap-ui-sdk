#include "BugReportWithoutEmailScreen.h"

const QString BugReportWithoutEmailScreen::SCREEN_NAME = "BugReportWithoutEmail";

BugReportWithoutEmailScreen::BugReportWithoutEmailScreen(QWidget *a_parent)
    : BugReportScreenBase(a_parent)
{
    this->create(m_ui);

    AdaptiveScreen::initScreen(this);
}

QString BugReportWithoutEmailScreen::screenName()
{
    return BugReportWithoutEmailScreen::SCREEN_NAME;
}

void BugReportWithoutEmailScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

    m_ui->edtEmail->hide();
    m_ui->lblCaption->hide();

    this->BugReportScreenBase::initVariantUi(a_widget);
}
