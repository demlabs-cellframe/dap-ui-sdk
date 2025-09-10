#include "DapCmdUserData.h"
#include <utility>

/// User login.
const QString DapCmdUserData::userParam =
        DapJsonParams::toString(DapJsonParams::USER);

/// User password.
const QString DapCmdUserData::passwordParam =
        DapJsonParams::toString(DapJsonParams::PASSWORD);

/// Server address.
const QString DapCmdUserData::addressParam =
        DapJsonParams::toString(DapJsonParams::ADDRESS);


void DapCmdUserData::handleResult(const QJsonObject& result)
{
    if(result.contains("license_term_till")) {
        qDebug() << "license_term_till " << result.value("license_term_till").toString();
        emit sigtLicenseTermTill(result.value("license_term_till").toString());
        return;
    } else if (result.contains("country_iso")) {
        qDebug() << "country_iso " << result.value("country_iso").toString();
        emit sigCoutryISOReceived(result.value("country_iso").toString());
        return;
    }

    if (result.value(userParam) != QJsonValue::Undefined &&
        result.value(passwordParam) != QJsonValue::Undefined &&
        result.value(addressParam) != QJsonValue::Undefined) {
        DapCmdUserData::userDataHandler(result.value(userParam), result.value(passwordParam), result.value(addressParam));
    } else {
        qWarning() << result;
    }
}

void DapCmdUserData::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}

///// Process command.
///// @param params Command parameters.
//void DapCmdUserDataHandler::handle(const QJsonObject* params) {
//    if (params->value(userParam) != QJsonValue::Undefined &&
//        params->value(passwordParam) != QJsonValue::Undefined &&
//        params->value(addressParam) != QJsonValue::Undefined) {
//        DapCmdUserDataHandler::userDataHandler(params->value(userParam), params->value(passwordParam), params->value(addressParam));
//    }
//}

/// Process and verify command parameters.
/// @param user User login.
/// @param password User password.
/// @param address Server address.
void DapCmdUserData::userDataHandler(const QJsonValue &user, const QJsonValue &password, const QJsonValue &address)
{
    QString bUser = user.toString(), bPassword = password.toString(), bAddress = address.toString();
    
    if (bUser.isNull() || bUser.isEmpty() || bPassword.isNull() || bPassword.isEmpty() || bAddress.isNull() || bAddress.isEmpty()) {
        qWarning() << "Something wrong can't parse user data";
        return;
    }
    emit sigUserData(bUser, bPassword, bAddress);
}

void DapCmdUserData::sendCmd()
{
    DapCmdClientAbstract::sendCmd(nullptr);
}
