#include "SignInWithTariffLink.h"

SignInWithTariffLink::SignInWithTariffLink(QWidget *a_parent)
    : SignInScreenSerialNumberBase (a_parent)
{
    this->create(m_ui);

    AdaptiveScreen::initScreen(this);
}

void SignInWithTariffLink::initVariantUi(QWidget *a_widget)
{
    m_ui->cbbTariff->popup()->setCaption(tr("Renew subscription"));
    m_ui->cbbTariff->popup()->listView()->setWidgetDelegateFactory(&TariffDelegate::create);
    m_ui->cbbTariff->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);

    connect(m_stt_serialKey_activated           , &QState::exited, [this]{

        m_ui->wgtFrameBottom->show();
    });


    connect(m_stt_serialKey_activated           , &QState::entered, [this]{

        m_ui->wgtFrameBottom->hide();
    });

#ifdef BUILD_VAR_DEFAULT
    connect(m_ui->cbbTariff, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), [=](const QString &a_url){
        QDesktopServices::openUrl(QUrl(a_url));
    });
#endif

    connect(m_ui->cbbTariff->buttonControll(),&QPushButton::clicked,this,&SignInWithTariffLink::tarriffScreenPopupOpened);

    SignInScreenSerialNumberBase::initVariantUi(a_widget);
}

#ifdef BUILD_VAR_GOOGLE
void SignInWithTariffLink::appendTariff(const QList<TariffItem> &a_tariffList, std::function<void (int)> func)
{
    for (const TariffItem& currentTarriff: a_tariffList)
        m_ui->cbbTariff->addItem(currentTarriff.URL, QVariant::fromValue(currentTarriff));

    connect(m_ui->cbbTariff, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), func);
}
#endif

void SignInWithTariffLink::appendTariff(const QList<TariffItem> &a_tariffList)
{
    for (const TariffItem& currentTarriff: a_tariffList)
        m_ui->cbbTariff->addItem(currentTarriff.URL, QVariant::fromValue(currentTarriff));
        
#ifdef BUILD_VAR_GOOGLE
    connect(m_ui->cbbTariff, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), [=](const QString &a_url){
        QDesktopServices::openUrl(QUrl(a_url));
    });
#endif
}

QList<CustomPopup *> SignInWithTariffLink::customPopups()
{
    return {
        m_ui->cbbServer->popup(),
        m_ui->cbbTariff->popup()
    };
}