#include "DapCmdUserDataSrv.h"

/// Process command.
/// @param params Command parameters.
void DapCmdUserDataSrv::handle(const QJsonObject *params)
{
    Q_UNUSED(params);
    
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
void DapCmdUserDataSrv::setUserData(const QString &user, const QString &password, const QString &address, quint16 a_port)
{
    mUser = user;
    mPassword = password;
    m_address = address;
    m_port = a_port;
}

void DapCmdUserDataSrv::sendLicenseTermTill(const QString &date)
{
    QJsonObject response;
    response["license_term_till"] = date;
    sendCmd(&response);
}
