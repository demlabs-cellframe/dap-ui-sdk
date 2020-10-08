#include "SignInWithTariffLink.h"

SignInWithTariffLink::SignInWithTariffLink(QWidget *a_parent)
    : SignInScreenSerialNumberBase (a_parent)
{
    this->create(m_ui);

    AdaptiveScreen::initScreen(this);
}

void SignInWithTariffLink::initVariantUi(QWidget *a_widget)
{
    m_ui->cbbTariff->popup()->setCaption("Renew subscription");
    m_ui->cbbTariff->popup()->listView()->setWidgetDelegateFactory(&TariffDelegate::create);
    m_ui->cbbTariff->setCaptionPolicy(ComboBox::CaptionPolicy::ShowAlways);

    connect(m_stt_serialKey_activated           , &QState::exited, [this]{

        m_ui->wgtFrameBottom->show();
    });


    connect(m_stt_serialKey_activated           , &QState::entered, [this]{

        m_ui->wgtFrameBottom->hide();
    });

    connect(m_ui->cbbTariff, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), [=](const QString &a_url){
        QDesktopServices::openUrl(QUrl(a_url));
    });

    connect(m_ui->cbbTariff->buttonControll(),&QPushButton::clicked,this,&SignInWithTariffLink::showPopupScreenTarriff);

    SignInScreenSerialNumberBase::initVariantUi(a_widget);
}

void SignInWithTariffLink::appendTariff(const QList<TariffItem> &a_tariffList)
{
    for (const TariffItem& currentTarriff: a_tariffList)
        m_ui->cbbTariff->addItem(currentTarriff.URL, QVariant::fromValue(currentTarriff));
}

QList<CustomPopup *> SignInWithTariffLink::customPopups()
{
    return {
        m_ui->cbbServer->popup(),
        m_ui->cbbTariff->popup()
    };
}
