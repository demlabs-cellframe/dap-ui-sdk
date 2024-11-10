#include "DapServiceDataLocal.h"
#include "DapSerialKeyData.h"
#include "DapSerialKeyHistory.h"

DapServiceDataLocal::DapServiceDataLocal() {}

DapServiceDataLocal *DapServiceDataLocal::instance()
{
    static DapServiceDataLocal s_instance;
    return &s_instance;
}

QVariant DapServiceDataLocal::getSetting(const QString &a_setting)
{
    return instance()->getValueSetting(a_setting);
}

void DapServiceDataLocal::saveSetting(const QString &a_setting, const QVariant &a_value)
{
    instance()->saveValueSetting(a_setting, a_value);
}

void DapServiceDataLocal::removeSetting(const QString &a_setting)
{
    instance()->removeValueSetting(a_setting);
}

void DapServiceDataLocal::setCountryISO(const QString& iso_code)
{
    DapBaseDataLocal::setCountryISO(iso_code);
    emit valueServiceDataLocalUpdated(QJsonObject{{JSON_COUNTRY_ISO_KEY, m_coutryISO}});
}

void DapServiceDataLocal::saveValueSetting(const QString &setting, const QVariant &value)
{
    DapBaseDataLocal::saveValueSetting(setting, value);
    QJsonObject result{{setting,  QJsonValue::fromVariant(value)}} ;
    emit valueServiceDataLocalUpdated(QJsonObject{{JSON_SETTINGS_KEY, result}});
}

void DapServiceDataLocal::removeValueSetting(const QString &setting)
{
    DapBaseDataLocal::removeValueSetting(setting);
    QJsonObject result{{setting,  QJsonObject()}};
    emit valueServiceDataLocalUpdated(QJsonObject{{JSON_SETTINGS_KEY, result}});
}

void DapServiceDataLocal::saveSerialKeyData()
{
    if (m_serialKeyData)
    {
        DapBaseDataLocal::saveSerialKeyData();
        emit valueServiceDataLocalUpdated(QJsonObject{{JSON_SERIAL_KEY_DATA_KEY, serialKeyDataToJson()}});
    }
}

void DapServiceDataLocal::resetSerialKeyData()
{
    if (m_serialKeyData)
    {
        DapBaseDataLocal::resetSerialKeyData();
        emit valueServiceDataLocalUpdated(QJsonObject{{JSON_SERIAL_KEY_DATA_KEY, QJsonObject()}});
    }
}

// void DapServiceDataLocal::saveBugReport()
// {
//     DapBaseDataLocal::saveBugReport();
//     emit valueServiceDataLocalUpdated(QJsonObject{{JSON_BUG_REPORT_HISTORY_KEY, bugReportHistoryToJson()}});
// }

void DapServiceDataLocal::savePendingSerialKey(QString a_serialkey)
{
    DapBaseDataLocal::savePendingSerialKey(a_serialkey);
    emit valueServiceDataLocalUpdated(createJsonObject(JSON_PENDING_SERIAL_KEY_KEY, m_pendingSerialKey));
}

void DapServiceDataLocal::setLogin(const QString &login)
{
    DapBaseDataLocal::setLogin(login);
    emit valueServiceDataLocalUpdated(createJsonObject(JSON_LOGIN_KEY, m_login));
}

void DapServiceDataLocal::setPassword(const QString &password)
{
    DapBaseDataLocal::setPassword(password);
    emit valueServiceDataLocalUpdated(createJsonObject(JSON_PASSWORD_KEY, m_password));
}

void DapServiceDataLocal::addNewSerialKey()
{
    auto key  = serialKeyData()->serialKey();
    if (serialKeyHistory()->getKeysHistory().contains(key))
    {
        return;
    }
    DapBaseDataLocal::addNewSerialKey();
    emit valueServiceDataLocalUpdated({{JSON_SERIAL_KEY_HISTORY_KEY, serialKeyHistoryToJson()}});
}

void DapServiceDataLocal::setBugReportHistory(const QJsonArray& list)
{
    DapBaseDataLocal::setBugReportHistory(list);
    saveBugReport();
}
