#include "DapCmdUserData.h"

/// Process command.
/// @param params Command parameters.
void DapCmdUserData::handle(const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
    
    QJsonObject response;
    response["user"] = mUser;
    response["password"] = mPassword;
    response["address"] = m_address;
    response["port"] = m_port;
    sendCmd(&response);
}

/// Set user data.
/// @param user User login.
/// @param password User password.
/// @param address Server address.
void DapCmdUserData::setUserData(const QString &user, const QString &password, const QString &address, quint16 a_port)
{
    mUser = user;
    mPassword = password;
    m_address = address;
    m_port = a_port;
}

void DapCmdUserData::sendLicenseTermTill(const QString &date)
{
    QJsonObject response;
    response["license_term_till"] = date;
    sendCmd(&response);
}

void DapCmdUserData::sendCountryISO(const QString &iso)
{
    QJsonObject response;
    response["country_iso"] = iso;
    sendCmd(&response);
}
