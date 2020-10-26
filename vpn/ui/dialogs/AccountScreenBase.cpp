#include "AccountScreenBase.h"

#include "TariffDelegate.h"

const QString AccountScreenBase::SCREEN_NAME = "Account";

AccountScreenBase::AccountScreenBase(QWidget *a_parent)
    : ScreenWithScreenPopupsAbstract(a_parent)
    #ifndef Q_OS_ANDROID
    , m_serialRemovalMessage(new SerialRemovalConfirmationMessage(this))
    #endif
{
    this->create(m_ui);
#ifndef Q_OS_ANDROID
    m_serialRemovalMessage->setWindowType(Qt::Dialog);
    m_serialRemovalMessage->setObjectName("ScreenMessagePopup");
    connect(m_serialRemovalMessage, &SerialRemovalConfirmationMessage::accepted, this, &AccountScreenBase::serialRemovalRequested);
#endif

    AdaptiveScreen::initScreen(this);
}

QString AccountScreenBase::screenName()
{
    return AccountScreenBase::SCREEN_NAME;
}

void AccountScreenBase::setState(ConnectionState a_state)
{
    Q_UNUSED(a_state)
}

void AccountScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

#ifdef Q_OS_ANDROID
  //  m_ui->lblCaption->setText("Renew subscription");
#else
    connect(m_ui->btnResetSerial, &QPushButton::clicked, m_serialRemovalMessage, &SerialRemovalConfirmationMessage::show);

    m_ui->cbbLicenceTariff->popup()->listView()->setWidgetDelegateFactory(&TariffDelegate::create);
    m_ui->cbbLicenceTariff->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    connect(m_ui->cbbLicenceTariff, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), [=](const QString &a_url){
        QDesktopServices::openUrl(QUrl(a_url));
    });

    m_ui->cbbBugReport->hide();
#endif

    this->ScreenWithScreenPopupsAbstract::initVariantUi(a_widget);
}

#ifndef Q_OS_ANDROID
void AccountScreenBase::setState(ActivationState a_activationState)
{
    m_ui->btnResetSerial->setEnabled(a_activationState == ActivationState::Activated);
}

void AccountScreenBase::appendTariff(const QList<TariffItem> &a_tariffList)
{
    for (const TariffItem& currentTarriff: a_tariffList)
        m_ui->cbbLicenceTariff->addItem(currentTarriff.URL, QVariant::fromValue(currentTarriff));
}
#endif

QList<CustomPopup *> AccountScreenBase::customPopups()
{
    return {
#ifndef Q_OS_ANDROID
        m_ui->cbbLicenceTariff->popup(),
        m_ui->cbbBugReport->popup()
#endif
    };
}
