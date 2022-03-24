#include "DapSignUpData.h"

#include "DapDataLocal.h"

DapSignUpData::DapSignUpData()
{
    this->loadDatas();
}

DapSignUpData *DapSignUpData::instance()
{
    static DapSignUpData s_instance;
    return &s_instance;
}

QString DapSignUpData::password() const
{
    return m_password;
}

QString DapSignUpData::email() const
{
    return m_email;
}

QString DapSignUpData::dataAnswer() const
{
    return m_data;
}

void DapSignUpData::setPassword(const QString &a_password)
{
    if (m_password == a_password)
        return;

    m_password = a_password;
//    emit this->passwordChanged(a_password);
}

void DapSignUpData::setEmail(const QString &a_email)
{
    if (m_email == a_email)
        return;

    m_email = a_email;
//    emit this->emailChanged(a_email);
}

void DapSignUpData::setDataAnswer(const QString &a_data)
{
    if (m_data == a_data)
        return;

    m_data = a_data;
//    emit this->passwordChanged(a_data);
}

void DapSignUpData::saveDatas() const
{
    DapDataLocal::instance()->saveEncryptedSetting(SETTINGS_TEXT_EMAIL  , this->email());
    DapDataLocal::instance()->saveEncryptedSetting(SETTINGS_TEXT_PASSWORD, this->password());
}

void DapSignUpData::loadDatas()
{
    this->setEmail  (DapDataLocal::instance()->getEncryptedSetting(SETTINGS_TEXT_EMAIL).toString());
    this->setPassword(DapDataLocal::instance()->getEncryptedSetting(SETTINGS_TEXT_PASSWORD).toString());
}
