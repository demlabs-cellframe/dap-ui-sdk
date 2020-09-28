#include "IndicatorsFrame.h"
#include "ui_IndicatorsFrame.h"

IndicatorsFrame::IndicatorsFrame(QWidget *a_parent)
    :QFrame(a_parent),
      m_ui(new Ui::IndicatorsFrame)
{
    m_ui->setupUi(this);
}

IndicatorsFrame::~IndicatorsFrame()
{
    delete m_ui;
}

void IndicatorsFrame::setAuthorized(bool a_authorized)
{
    m_ui->autorizedCheckBox->setChecked(a_authorized);
}

void IndicatorsFrame::setStreamOpened(bool a_streamOpened)
{
    m_ui->streamOpenedCheckBox->setChecked(a_streamOpened);
}

void IndicatorsFrame::setVirtualNetwork(bool a_virtualNetwork)
{
    m_ui->virtualNetworkCheckBox->setChecked(a_virtualNetwork);
}
