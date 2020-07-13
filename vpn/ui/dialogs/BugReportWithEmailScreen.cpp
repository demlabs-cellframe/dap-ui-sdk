#include "BugReportWithEmailScreen.h"

const QString BugReportWithEmailScreen::SCREEN_NAME = "BugReportWithEmail";

BugReportWithEmailScreen::BugReportWithEmailScreen(QWidget *a_parent)
    : BugReportScreenBase(a_parent)
{
    this->create(m_ui);

    AdaptiveScreen::initScreen(this);
}

QString BugReportWithEmailScreen::screenName()
{
    return BugReportWithEmailScreen::SCREEN_NAME;
}

void BugReportWithEmailScreen::setEmail(const QString &a_email)
{
    m_ui->edtEmail->setText(a_email);
}

void BugReportWithEmailScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget);

    this->BugReportScreenBase::initVariantUi(a_widget);
#ifdef Q_OS_ANDROID
    m_ui->edtEmail->setPlaceholderText(tr("e-mail"));
#elif defined(Q_OS_MAC)
    m_ui->edtEmail->setAttribute(Qt::WA_MacShowFocusRect,false);
#endif

    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption) ;
    m_ui->edtEmail->setValidator(new QRegularExpressionValidator(rx, this));

    connect(this, &BugReportWithEmailScreen::wrongEmail, [=]()
    {
        Utils::setPropertyAndUpdateStyle(m_ui->edtEmail, Properties::WRONG, true);
    });

    connect(m_ui->edtEmail, &QLineEdit::textChanged, [=](const QString &a_text)
    {
        Utils::setPropertyAndUpdateStyle(m_ui->edtEmail, Properties::WRONG, false);
        emit this->emailChanged(a_text);
    });
}

bool BugReportWithEmailScreen::checkEmail()
{
    if (!m_ui->edtEmail->hasAcceptableInput())
    {
        emit this->wrongEmail();
        return false;
    }
    else
        return true;
}

bool BugReportWithEmailScreen::checkFields()
{
    return true;

    return (this->checkEmail() && this->BugReportScreenBase::checkFields());
}
