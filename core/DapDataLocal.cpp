#include "DapDataLocal.h"
#include "DapSerialKeyData.h"
#include "qjsondocument.h"

DapDataLocal::DapDataLocal()
    :DapBaseDataLocal()
{
    QStringList keys = m_settings->allKeys();
    if(!keys.isEmpty())
    {
        if(!keys.contains(MIGRATION_KEY))
        {
            m_needMigration = true;
        }
    }
}

DapDataLocal *DapDataLocal::instance()
{
    static DapDataLocal instance;
    return &instance;
}

void DapDataLocal::saveMigrate()
{
    DapBaseDataLocal::saveValueSetting(MIGRATION_KEY, true);
}

QVariant DapDataLocal::getSetting(const QString &a_setting)
{
    return instance()->getValueSetting(a_setting);
}

void DapDataLocal::saveSetting(const QString &a_setting, const QVariant &a_value)
{
    instance()->saveValueSetting(a_setting, a_value);
}

void DapDataLocal::removeSetting(const QString &a_setting)
{
    instance()->removeValueSetting(a_setting);
}

QVariant DapDataLocal::getValueSetting(const QString &a_setting)
{
    return m_settingsMap[a_setting];
}

void DapDataLocal::saveValueSetting(const QString &setting, const QVariant &value)
{
    if(m_settingsMap.value(setting) != value)
    {
        m_settingsMap[setting] = value;
        QJsonObject result{{setting, QJsonValue::fromVariant(value)}};
        emit valueDataLocalUpdated(QJsonObject{{JSON_SETTINGS_KEY, result}});
    }
}

void DapDataLocal::removeValueSetting(const QString &setting)
{
    if(m_settingsMap.contains(setting))
    {
        m_settingsMap.remove(setting);
        QJsonObject result{{setting, QJsonObject()}};
        emit valueDataLocalUpdated(QJsonObject{{JSON_SETTINGS_KEY, result}});
    }
}

void DapDataLocal::setCountryISO(const QString& iso_code)
{
    if(m_coutryISO != iso_code)
    {
        DapBaseDataLocal::setCountryISO(iso_code);
        emit valueDataLocalUpdated(createJsonObject(JSON_COUNTRY_ISO_KEY, m_coutryISO));
    }
}

void DapDataLocal::saveSerialKeyData()
{
    if (m_serialKeyData)
    {
        emit valueDataLocalUpdated(QJsonObject{{JSON_SERIAL_KEY_DATA_KEY, serialKeyDataToJson()}});
    }
}

void DapDataLocal::resetSerialKeyData()
{
    if (m_serialKeyData)
    {
        m_serialKeyData->reset();
    }
}

void DapDataLocal::loadBugReport()
{}

void DapDataLocal::saveBugReport()
{
    emit valueDataLocalUpdated(QJsonObject{{JSON_BUG_REPORT_HISTORY_KEY, bugReportHistoryToJson()}});
}

void DapDataLocal::savePendingSerialKey(QString a_serialkey)
{
    m_pendingSerialKey = a_serialkey;
    emit valueDataLocalUpdated(createJsonObject(JSON_PENDING_SERIAL_KEY_KEY, m_pendingSerialKey));
}

void DapDataLocal::setLogin(const QString &login)
{
    DapBaseDataLocal::setLogin(login);
    emit valueDataLocalUpdated(createJsonObject(JSON_LOGIN_KEY, m_login));
}

void DapDataLocal::setPassword(const QString &password)
{
    DapBaseDataLocal::setPassword(password);
    emit valueDataLocalUpdated(createJsonObject(JSON_PASSWORD_KEY, m_password));
}

void DapDataLocal::saveAuthorizationData()
{
    saveValueSetting(TEXT_LOGIN, login());
    saveValueSetting(TEXT_PASSWORD, password());
}

void DapDataLocal::saveKeysHistory()
{}

void DapDataLocal::setSettings(const QJsonObject &json)
{
    for(const auto& key: json.keys())
    {
        if(key == "remove")
        {
            m_settingsMap.remove(json[key].toString());
        }
        else
        {
            if(TEXT_SERIAL_KEY == key || TEXT_PENDING_SERIAL_KEY == key || TEXT_SERIAL_KEY_HISTORY == key ||
                TEXT_BUGREPORT_HISTORY == key)
            {
                continue;
            }
            else if(key == ROUTING_EXCEPTIONS_LIST)
            {
                m_settingsMap[key] = json[key].toObject();
            }
            else if(key == NOTIFICATION_HISTORY)
            {
                QJsonArray jsonArray = json[key].toArray();
                QByteArray result = QJsonDocument(jsonArray).toJson(QJsonDocument::JsonFormat::Compact);
                m_settingsMap[key] = result;
            }
            else
            {
                m_settingsMap[key] = json[key].toVariant();
            }
        }
    }
}

void DapDataLocal::updateCdbList (const DapCdbServerList &a_newCdbList)
{
    Q_UNUSED(a_newCdbList)
}

void DapDataLocal::dataFromCommand(const QJsonObject& object)
{
    fromJson(object);
    if(object.contains("action"))
    {
        QString action = object["action"].toString();
        if(action == "setAll")
        {
            emit allDataReceived();
        }
    }
}
