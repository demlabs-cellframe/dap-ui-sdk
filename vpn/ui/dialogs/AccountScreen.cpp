#include "AccountScreen.h"

#include "TariffDelegate.h"

const QString AccountScreen::SCREEN_NAME = "Account";

AccountScreen::AccountScreen(QWidget *a_parent)
    : ScreenWithScreenPopupsAbstract(a_parent)
    #ifndef Q_OS_ANDROID
    , m_serialRemovalMessage(new SerialRemovalConfirmationMessage(this))
    #endif
{
    this->create(m_ui);
#ifndef Q_OS_ANDROID
    m_serialRemovalMessage->setWindowType(Qt::Dialog);
    m_serialRemovalMessage->setObjectName("ScreenMessagePopup");
    connect(m_serialRemovalMessage, &SerialRemovalConfirmationMessage::accepted, this, &AccountScreen::serialRemovalRequested);
#endif

    AdaptiveScreen::initScreen(this);
}

QString AccountScreen::screenName()
{
    return AccountScreen::SCREEN_NAME;
}

void AccountScreen::setState(ConnectionState a_state)
{
    Q_UNUSED(a_state)
}

#ifdef Q_OS_ANDROID
void AccountScreen::tariffUpdate(DapShopManager::Products tariff, DapShopManager::ProductState state)
{
    QPushButton* button = nullptr;
    switch (tariff) {
    case DapShopManager::PRODUCT_1MONTH_KEY:
        button = m_ui->btnMonthPrice;
        break;
    case DapShopManager::PRODUCT_6MONTHS_KEY:
        button = m_ui->btnSixMonthPrice;
        break;
    case DapShopManager::PRODUCT_YEAR_KEY:
        button = m_ui->btnYearPrice;
        break;
    default:
        return; // unknown tariff
        break;
    }

    switch (state) {
    case DapShopManager::STATE_AVAILABLE:
        button->setEnabled(true);
        break;
    case DapShopManager::STATE_UNAVAILABLE:
        button->setEnabled(false);
        break;
    case DapShopManager::STATE_PURCHASED:
        //TODO:
        break;
    }
}
#endif

void AccountScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

#ifdef Q_OS_ANDROID
    m_ui->lblCaption->setText("Renew subscription");
#else
    connect(m_ui->btnResetSerial, &QPushButton::clicked, m_serialRemovalMessage, &SerialRemovalConfirmationMessage::show);

    m_ui->cbbLicenceKey->popup()->listView()->setWidgetDelegateFactory(&TariffDelegate::create);
    m_ui->cbbLicenceKey->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);
    connect(m_ui->cbbLicenceKey, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), [] (const QString &a_current)
    {
        QDesktopServices::openUrl(QUrl(a_current));
    });

#endif

    this->ScreenWithScreenPopupsAbstract::initVariantUi(a_widget);
}

#ifndef Q_OS_ANDROID
void AccountScreen::setState(ActivationState a_activationState)
{
    m_ui->btnResetSerial->setEnabled(a_activationState == ActivationState::Activated);
}

void AccountScreen::appendTariff(const QList<TariffItem> &a_tariffList)
{
    for (const TariffItem& currentTarriff: a_tariffList)
        m_ui->cbbLicenceKey->addItem(currentTarriff.urlTariff, QVariant::fromValue(currentTarriff));
}
#endif

QList<CustomPopup *> AccountScreen::customPopups()
{
    return {
#ifndef Q_OS_ANDROID
        m_ui->cbbLicenceKey->popup(),
        m_ui->cbbBugReport->popup()
#endif
    };
}

#ifdef Q_OS_ANDROID
void AccountScreen::showMessage(const QString& msg) {
    m_ui->label->setText(msg);
}

// this function used for take aut BRAND-specific code
QPushButton* AccountScreen::getTariffButton(DapShopManager::Products tariff) {
    QPushButton* ptr = nullptr;
    switch (tariff) {
    case DapShopManager::PRODUCT_1MONTH_KEY:
        ptr = m_ui->btnMonthPrice;
        break;
    case DapShopManager::PRODUCT_6MONTHS_KEY:
        ptr = m_ui->btnSixMonthPrice;
        break;
    case DapShopManager::PRODUCT_YEAR_KEY:
        ptr = m_ui->btnYearPrice;
        break;
    default:
        break;
    }
    return ptr;
}
#endif

