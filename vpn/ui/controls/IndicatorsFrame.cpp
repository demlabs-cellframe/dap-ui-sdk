#include "IndicatorsFrame.h"

IndicatorsFrame::IndicatorsFrame(QWidget *a_parent)
    :QFrame(a_parent)
{
    m_layout = new QVBoxLayout(this);

    m_authorized = new QCheckBox(tr("Authorized"));
    m_authorized->setObjectName("autorizedCheckBox");
    //m_authorized->setEnabled(false);

    m_streamOpened = new QCheckBox(tr("Stream Established"));
    m_streamOpened->setObjectName("streamOpenedCheckBox");

    m_virtualNetwork = new QCheckBox(tr("Network Settings Present"));
    m_virtualNetwork->setObjectName("virtualNetworkCheckBox");

    m_layout->addWidget(m_authorized);
    m_layout->addWidget(m_streamOpened);
    m_layout->addWidget(m_virtualNetwork);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

void IndicatorsFrame::setAuthorized(bool a_authorized)
{
    m_authorized->setChecked(a_authorized);
}

void IndicatorsFrame::setStreamOpened(bool a_streamOpened)
{
    m_streamOpened->setChecked(a_streamOpened);
}

void IndicatorsFrame::setVirtualNetwork(bool a_virtualNetwork)
{
    m_virtualNetwork->setChecked(a_virtualNetwork);
}
