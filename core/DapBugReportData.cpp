#include "DapBugReportData.h"

#include "DapDataLocal.h"

DapBugReportData::DapBugReportData()
{
    this->loadDatas();
}

DapBugReportData::~DapBugReportData()
{
    this->saveDatas();
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

QString DapBugReportData::defaultEmail() const
{
    return DapDataLocal::instance()->getEncryptedSetting(DapBaseDataLocal::TEXT_LOGIN).toString();
}

QString DapBugReportData::lastSentNumber() const
{
    return m_lastSentNumber;
}

void DapBugReportData::setMessage(const QString &a_message)
{
    if (m_message == a_message)
        return;

    m_message = a_message;
    emit this->messageChanged(a_message);
}

void DapBugReportData::setEmail(const QString &a_email)
{
    if (m_email == a_email)
        return;

    m_email = a_email;
//    emit this->emailChanged(a_email);
}

///Return last successfull sent bug report number
/// @return QString() if was error
void DapBugReportData::setLastSentNumber(const QString& a_lastSentNumber /*= {})*/)
{
    if (m_lastSentNumber == a_lastSentNumber && a_lastSentNumber != "")
           return;

    m_lastSentNumber = a_lastSentNumber;
    emit this->lastSentNumberChanged(a_lastSentNumber);
}

void DapBugReportData::saveDatas() const
{
    DapDataLocal::instance()->saveEncryptedSetting(SETTINGS_TEXT_EMAIL  , this->email());
    DapDataLocal::instance()->saveEncryptedSetting(SETTINGS_TEXT_MESSAGE, this->message());
    DapDataLocal::instance()->saveEncryptedSetting(SETTINGS_TEXT_LAST_SENT_NUMBER, this->lastSentNumber());
}

void DapBugReportData::loadDatas()
{
    this->setEmail  (DapDataLocal::instance()->getEncryptedSetting(SETTINGS_TEXT_EMAIL).toString());
    this->setMessage(DapDataLocal::instance()->getEncryptedSetting(SETTINGS_TEXT_MESSAGE).toString());
    this->setMessage(DapDataLocal::instance()->getEncryptedSetting(SETTINGS_TEXT_LAST_SENT_NUMBER).toString());
}


