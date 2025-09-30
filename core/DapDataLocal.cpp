#include "DapDataLocal.h"
#include "DapSerialKeyData.h"
#include "DapSerialKeyHistory.h"
#include "qjsondocument.h"
#include <QDate>
#include <QDateTime>
#include <QDebug>

DapDataLocal::DapDataLocal()
    :DapBaseDataLocal()
{
    connect(serialKeyData(), &DapSerialKeyData::serialKeyToSave,
            this, &DapDataLocal::saveSerialKeyData);
    initSettings();
    initData();
    initAuthData();

    QStringList keys = m_settings->allKeys();

    if(keys.contains(SETTING_THEME)) {
        m_settingsMap[SETTING_THEME] = m_settings->value(SETTING_THEME);
    }
    
    // Load noCDB data into memory cache if it exists
    if(keys.contains(NOCDB_DATA)) {
        m_settingsMap[NOCDB_DATA] = m_settings->value(NOCDB_DATA);
        qDebug() << "[DapDataLocal] Loaded NOCDB_DATA into m_settingsMap";
    }
    if(!keys.contains(MIGRATION_KEY)) {
        qDebug() << "[DapDataLocal] Data needs to be migrated";
        m_needMigration = true;
    }
}

DapDataLocal *DapDataLocal::instance() {
    static DapDataLocal instance;
    return &instance;
}

QVariant DapDataLocal::getSetting(const QString &a_setting) {
    return instance()->getValueSetting(a_setting);
}

void DapDataLocal::saveSetting(const QString &a_setting, const QVariant &a_value) {
    instance()->saveValueSetting(a_setting, a_value);
}

void DapDataLocal::removeSetting(const QString &a_setting) {
    instance()->removeValueSetting(a_setting);
}

QVariant DapDataLocal::getValueSetting(const QString &a_setting) {
    return m_settingsMap[a_setting];
}

void DapDataLocal::saveValueSetting(const QString &setting, const QVariant &value) {
    if(m_settingsMap.value(setting) != value) {
        m_settingsMap[setting] = value;
        if(setting != SETTING_THEME) {
            QJsonObject result;
            
            // Special handling for NOTIFICATION_HISTORY to apply filtering before sending to service
            if (setting == NOTIFICATION_HISTORY) {
                // Apply filtering similar to settingsToJson() method
                QByteArray source = value.toByteArray();
                if (!source.isEmpty()) {
                    // Parse current notification history
                    QJsonArray originalNotifications = QJsonDocument::fromJson(source).array();
                    QJsonArray filteredNotifications;
                    
                    // Filter notifications: remove older than 3 days and limit to 50 entries
                    QDate threeDaysAgo = QDate::currentDate().addDays(-3);
                    int notificationCount = 0;
                    const int MAX_NOTIFICATIONS = 50;
                    
                    // Iterate through notifications and apply filters
                    for (const auto &item : qAsConst(originalNotifications))
                    {
                        QJsonObject notification = item.toObject();
                        
                        // Check if this is a title entry
                        bool isTitle = notification.value("isTitle").toBool();
                        
                        if (isTitle)
                        {
                            // For title entries, check the date field
                            QString dateString = notification.value("date").toString();
                            if (!dateString.isEmpty())
                            {
                                QDate titleDate = QDate::fromString(dateString, "dd.MM.yyyy");
                                if (titleDate.isValid() && titleDate >= threeDaysAgo)
                                {
                                    filteredNotifications.append(notification);
                                    notificationCount++;
                                }
                            }
                        }
                        else
                        {
                            // For notification entries, check the created field
                            QString createdString = notification.value("created").toString();
                            if (!createdString.isEmpty())
                            {
                                QDateTime createdDateTime = QDateTime::fromString(createdString, "hh:mm:ss dd.MM.yyyy");
                                if (createdDateTime.isValid() && createdDateTime.date() >= threeDaysAgo)
                                {
                                    filteredNotifications.append(notification);
                                    notificationCount++;
                                }
                            }
                        }
                        
                        // Stop if we've reached the maximum number of notifications
                        if (notificationCount >= MAX_NOTIFICATIONS)
                        {
                            qDebug() << "[DapDataLocal][saveValueSetting] NOTIFICATION_HISTORY truncated to" << MAX_NOTIFICATIONS << "entries";
                            break;
                        }
                    }
                    
                    // Convert filtered notifications back to compact JSON
                    QByteArray filteredResult = QJsonDocument(filteredNotifications).toJson(QJsonDocument::JsonFormat::Compact);
                    
                    // Check if the result is too large and truncate further if needed
                    const int MAX_NOTIFICATION_JSON_SIZE = 8000; // Leave some room for other settings
                    if (filteredResult.size() > MAX_NOTIFICATION_JSON_SIZE)
                    {
                        qWarning() << "[DapDataLocal][saveValueSetting] NOTIFICATION_HISTORY JSON too large (" 
                                   << filteredResult.size() << " bytes), truncating further";
                        
                        // Remove entries from the end until size is acceptable
                        while (filteredNotifications.size() > 0 && filteredResult.size() > MAX_NOTIFICATION_JSON_SIZE)
                        {
                            filteredNotifications.removeLast();
                            filteredResult = QJsonDocument(filteredNotifications).toJson(QJsonDocument::JsonFormat::Compact);
                        }
                        
                        qDebug() << "[DapDataLocal][saveValueSetting] NOTIFICATION_HISTORY truncated to" 
                                 << filteredNotifications.size() << "entries, size:" << filteredResult.size() << "bytes";
                    }
                    
                    // Update stored value with filtered data
                    m_settingsMap[setting] = filteredResult;
                    
                    result.insert(setting, QJsonValue::fromVariant(QVariant(filteredResult)));
                }
                else
                {
                    result.insert(setting, QJsonValue::fromVariant(value));
                }
            }
            else
            {
                result.insert(setting, QJsonValue::fromVariant(value));
            }
            
            emit valueDataLocalUpdated(QJsonObject{{JSON_SETTINGS_KEY, result}});
        } else {
            DapBaseDataLocal::saveValueSetting(setting,value);
        }
    }
}

void DapDataLocal::removeValueSetting(const QString &setting) {
    if(m_settingsMap.contains(setting)) {
        m_settingsMap.remove(setting);
        QJsonObject result{{setting, QJsonObject()}};
        emit valueDataLocalUpdated(QJsonObject{{JSON_SETTINGS_KEY, result}});
    }
}

void DapDataLocal::setCountryISO(const QString& iso_code) {
    if(m_coutryISO != iso_code) {
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
    QStringList keys = json.keys();
    qDebug() << "[DapDataLocal][setSettings] New value of the settings keys - " << keys;
    for(const auto& key: keys)
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
            else if(key == NOTIFICATION_HISTORY || key == NODE_ORDER_HISTORY)
            {
                if (key == NOTIFICATION_HISTORY) {
                    // Apply filtering for NOTIFICATION_HISTORY
                    QString notificationString = json[key].toString();
                    auto jsonArray = QJsonDocument::fromJson(notificationString.toUtf8());
                    QJsonArray originalNotifications = jsonArray.array();
                    QJsonArray filteredNotifications;
                    
                    // Filter notifications: remove older than 3 days and limit to 50 entries
                    QDate threeDaysAgo = QDate::currentDate().addDays(-3);
                    int notificationCount = 0;
                    const int MAX_NOTIFICATIONS = 50;
                    
                    // Iterate through notifications and apply filters
                    for (const auto &item : qAsConst(originalNotifications))
                    {
                        QJsonObject notification = item.toObject();
                        
                        // Check if this is a title entry
                        bool isTitle = notification.value("isTitle").toBool();
                        
                        if (isTitle)
                        {
                            // For title entries, check the date field
                            QString dateString = notification.value("date").toString();
                            if (!dateString.isEmpty())
                            {
                                QDate titleDate = QDate::fromString(dateString, "dd.MM.yyyy");
                                if (titleDate.isValid() && titleDate >= threeDaysAgo)
                                {
                                    filteredNotifications.append(notification);
                                    notificationCount++;
                                }
                            }
                        }
                        else
                        {
                            // For notification entries, check the created field
                            QString createdString = notification.value("created").toString();
                            if (!createdString.isEmpty())
                            {
                                QDateTime createdDateTime = QDateTime::fromString(createdString, "hh:mm:ss dd.MM.yyyy");
                                if (createdDateTime.isValid() && createdDateTime.date() >= threeDaysAgo)
                                {
                                    filteredNotifications.append(notification);
                                    notificationCount++;
                                }
                            }
                        }
                        
                        // Stop if we've reached the maximum number of notifications
                        if (notificationCount >= MAX_NOTIFICATIONS)
                        {
                            qDebug() << "[DapDataLocal][setSettings] NOTIFICATION_HISTORY truncated to" << MAX_NOTIFICATIONS << "entries";
                            break;
                        }
                    }
                    
                    // Convert filtered notifications back to compact JSON
                    QByteArray filteredResult = QJsonDocument(filteredNotifications).toJson(QJsonDocument::JsonFormat::Compact);
                    
                    // Check if the result is too large and truncate further if needed
                    const int MAX_NOTIFICATION_JSON_SIZE = 8000; // Leave some room for other settings
                    if (filteredResult.size() > MAX_NOTIFICATION_JSON_SIZE)
                    {
                        qWarning() << "[DapDataLocal][setSettings] NOTIFICATION_HISTORY JSON too large (" 
                                   << filteredResult.size() << " bytes), truncating further";
                        
                        // Remove entries from the end until size is acceptable
                        while (filteredNotifications.size() > 0 && filteredResult.size() > MAX_NOTIFICATION_JSON_SIZE)
                        {
                            filteredNotifications.removeLast();
                            filteredResult = QJsonDocument(filteredNotifications).toJson(QJsonDocument::JsonFormat::Compact);
                        }
                        
                        qDebug() << "[DapDataLocal][setSettings] NOTIFICATION_HISTORY truncated to" 
                                 << filteredNotifications.size() << "entries, size:" << filteredResult.size() << "bytes";
                    }
                    
                    m_settingsMap[key] = DapUtils::toByteArray(filteredResult);
                    
                    qDebug() << "[DapDataLocal][setSettings] NOTIFICATION_HISTORY filtered:" 
                             << originalNotifications.size() << "->" << filteredNotifications.size() << "entries";
                }
                else {
                    // Handle NODE_ORDER_HISTORY as before
                    auto jsonArray = QJsonDocument::fromJson(json[key].toString().toUtf8());
                    QByteArray result = QJsonDocument (jsonArray).toJson (QJsonDocument::JsonFormat::Compact);
                    m_settingsMap[key] = DapUtils::toByteArray(result);
                }
            }
            else
            {
                m_settingsMap[key] = json[key].toVariant();
            }
        }
    }
}

void DapDataLocal::dataFromCommand(const QJsonObject& object)
{
    QString action;
    if(object.contains("action"))
    {
        action = object["action"].toString();
    }
    bool isAll = action == "setAll";

    qDebug() << "[DapDataLocal] [dataFromCommand] action: " << action;

    if(isAll && object.contains(JSON_SERIAL_KEY_HISTORY_KEY)){
        QStringList keysList = m_serialKeyHistory->getKeysHistory();
        const auto& serialArray = object[JSON_SERIAL_KEY_HISTORY_KEY].toArray();
        if(!keysList.isEmpty() && serialArray.isEmpty()) {
            emit reMigrationSignal();
        }
        else {
            fromJson(object);
        }
    } else if (object.contains(JSON_SERIAL_KEY_DATA_KEY)){
        QJsonObject serialKeyData = object[JSON_SERIAL_KEY_DATA_KEY].toObject();
        DapDataLocal::instance()->serialKeyData()->setFromJson(serialKeyData);
        emit serialKeyDataUpdateFromService();
    }
    else {
        fromJson(object);
    }

    if(isAll) {
        emit allDataReceived();
    }
}

void DapDataLocal::saveEncryptedSetting(const QString &a_setting, const QVariant &a_value)
{
    DapBaseDataLocal::saveEncryptedSetting(a_setting, a_value);
}

void DapDataLocal::saveEncryptedSetting(const QString &setting, const QByteArray &value)
{
    QByteArray result = DapUtils::fromByteArray<QByteArray>(value);
    saveValueSetting(setting, result);
}

bool DapDataLocal::loadEncryptedSettingString(const QString &a_setting, QByteArray& a_outString)
{
    QVariant varSettings = getValueSetting(a_setting);

    if (!varSettings.isValid() || !varSettings.canConvert<QByteArray>())
        return false;

    QByteArray encryptedString = varSettings.toByteArray();
    if (encryptedString.isEmpty())
    {
        a_outString = "";
        return true;
    }
    a_outString = encryptedString;
    return true;
}
