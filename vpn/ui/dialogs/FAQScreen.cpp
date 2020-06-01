#include "FAQScreen.h"

const QString FAQScreen::SCREEN_NAME = "FAQ";

FAQScreen::FAQScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::FAQScreen>();

}

QString FAQScreen::screenName()
{
    return FAQScreen::SCREEN_NAME;
}

void FAQScreen::setState(ConnectionStates a_state)
{
    Q_UNUSED(a_state)


}

void FAQScreen::initVariantUi(QWidget *a_widget)
{
    QVBoxLayout *vltFAQMain = a_widget->findChild<QVBoxLayout*>("vltFAQMain");  Q_ASSERT(vltFAQMain);
    QScrollArea *scaFAQ = a_widget->findChild<QScrollArea*>(SCA_FAQ); Q_ASSERT(scaFAQ);
#ifndef Q_OS_ANDROID
    QLabel *lblCaption = a_widget->findChild<QLabel*>("lblCaption");            Q_ASSERT(lblCaption);
    vltFAQMain->setAlignment(lblCaption,Qt::AlignCenter);
    vltFAQMain->setAlignment(scaFAQ,Qt::AlignCenter);
#else
    QScroller::grabGesture(scaFAQ->viewport(), QScroller::LeftMouseButtonGesture);
#endif
    m_dataModel = new QList<FAQDataModel>();
    //set model
    FAQDataModel tmpModel;
    tmpModel.text = "Who is behind KelVPN?";
    tmpModel.aboutText = "We are a private startup Cellframe Inc. We’re passionate about quantum physics and Internet security. When Wikipedia was blocked in Turkey in 2017 we took action. We created an instrument that helps people to maintain freedom of information. It is Cellframe – the blockchain protocol with post-quantum encryption. This technology is the core of KelVPN";
    m_dataModel->append(tmpModel);

    tmpModel.text = "How can I contact you?";
    tmpModel.aboutText = "You can contact us at info@kelvpn.com If you like to know more about Cellframe follow us at Telegram";
    m_dataModel->append(tmpModel);
    tmpModel.text = "When is the iOS application available?";
    tmpModel.aboutText = "Our team works hard to deliver it in November. Сan you send us your request at info@kelvpn.com so we could contact you when it’s released?";
    m_dataModel->append(tmpModel);
    tmpModel.text = "Why is blockchain VPN better?";
    tmpModel.aboutText = "It is decentralized. Unlike other VPNs, it doesn’t have a central point of control. KelVPN combines the traffic of many computers that agreed to be nodes. So, each computer of the network acts as a server. All your traffic is distributed between different nodes of the network to provide you with the best speed. This also means that decentralized KelVPN cannot collect logs, spy or sell your data.";
    m_dataModel->append(tmpModel);

    tmpModel.text = "Why is your VPN safer than others?";
    tmpModel.aboutText ="It has quantum encryption on board to protect your private data even from supercomputer attacks. We also developed our own modules for tunneling traffic. It helps to avoid errors and other vulnerabilities that other VPNs have.";

    m_dataModel->append(tmpModel);

    m_wgtScroll = new QWidget() ;
    m_VericalLayout = new QVBoxLayout();
    m_listWgt = new QList<QWidget *>();
    m_wgtScroll->setObjectName("wgtScrollArea");


    for(int i = 0; i < m_dataModel->size();i++)
    {
        m_listWgt ->append(new FAQWidget(m_dataModel->at(i)));
        m_VericalLayout->addWidget(m_listWgt->at(i));

    }
    QWidget *bottom = new QWidget();
    m_VericalLayout->addWidget(bottom);

    m_VericalLayout->setSpacing(0);
    m_VericalLayout->setMargin(0);
    m_wgtScroll->setLayout(m_VericalLayout);

    scaFAQ->setWidget(m_wgtScroll);

}

