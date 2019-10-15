#include "DapCmdUserData.h"

/// Process command.
/// @param params Command parameters.
void DapCmdUserData::handle(const QJsonObject *params)
{
    Q_UNUSED(params);
    
    QJsonObject response;
    response["user"] = mUser;
    response["password"] = mPassword;
    response["address"] = mAddress;
    sendCmd(&response);
}

/// Set user data.
/// @param user User login.
/// @param password User password.
/// @param address Server address.
void DapCmdUserData::setUserData(const QString &user, const QString &password, const QString &address)
{
    mUser = user;
    mPassword = password;
    mAddress = address;
}
