#include "DapBugReportData.h"

#include "DapDataLocal.h"

DapBugReportData::DapBugReportData()
{
    this->loadDatas();
}

DapBugReportData *DapBugReportData::instance()
{
    static DapBugReportData s_instance;
    return &s_instance;
}

QString DapBugReportData::message() const
{
    return m_message;
}

QString DapBugReportData::email() const
{
    return m_email;
}

void DapBugReportData::setMessage(const QString &a_message)
{
    if (m_message == a_message)
        return;

    m_message = a_message;
//    emit this->messageChanged(a_message);
}

void DapBugReportData::setEmail(const QString &a_email)
{
    if (m_email == a_email)
        return;

    m_email = a_email;
//    emit this->emailChanged(a_email);
}

void DapBugReportData::saveDatas() const
{
    DapDataLocal::saveSetting(SETTINGS_TEXT_EMAIL  , this->email());
    DapDataLocal::saveSetting(SETTINGS_TEXT_MESSAGE, this->message());
}

void DapBugReportData::loadDatas()
{
    this->setEmail  (DapDataLocal::getSetting(SETTINGS_TEXT_EMAIL)  .toString());
    this->setMessage(DapDataLocal::getSetting(SETTINGS_TEXT_MESSAGE).toString());
}
