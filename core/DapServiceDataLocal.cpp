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
    QJsonObject result{{setting,  QJsonValue::fromVariant(value)}};
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

void DapServiceDataLocal::setMigrationInfo(const QJsonObject& object)
{
    auto setMigrateDataString = [] (QString &data, const QJsonObject& object, const QString& key) -> bool
    {
        QString value = object[key].toString();
        if(value == data)
        {
            return false;
        }
        if(!data.isEmpty())
        {
            qInfo() << "The data from the client differs from the data in the service. The client's data is being set."
                       << " serviceData: " << data << " clientData: " << value;
        }

        data = value;
        return true;
    };

    if(object.contains(JSON_SETTINGS_KEY))              setSettings(object[JSON_SETTINGS_KEY].toObject());

    if(object.contains(JSON_KELVPN_PUB_KEY))            setMigrateDataString(m_kelvpnPub, object, JSON_KELVPN_PUB_KEY);
    if(object.contains(JSON_NETWORK_DEFAULT_KEY))       setMigrateDataString(m_networkDefault, object, JSON_NETWORK_DEFAULT_KEY);
    if(object.contains(JSON_URL_SITE_KEY))              setMigrateDataString(m_urlSite, object, JSON_URL_SITE_KEY);
    if(object.contains(JSON_COUNTRY_ISO_KEY))           setMigrateDataString(m_coutryISO, object, JSON_COUNTRY_ISO_KEY);
    bool isNewLogin = false;
    bool isNewPassword = false;
    if(object.contains(JSON_LOGIN_KEY))
    {
        isNewLogin = setMigrateDataString(m_login, object, JSON_LOGIN_KEY);
    }
    if(object.contains(JSON_PASSWORD_KEY))
    {
        isNewPassword = setMigrateDataString(m_password, object, JSON_PASSWORD_KEY);
    }
    if(isNewLogin || isNewPassword)
    {
        saveAuthorizationData();
    }
    if(object.contains(JSON_SERIAL_KEY_KEY))              setMigrateDataString(m_serialKey,  object, JSON_SERIAL_KEY_KEY);
    if(object.contains(JSON_MIN_DASHBOARD_VERSION_KEY))   setMigrateDataString(m_minDashboardVersion, object, JSON_MIN_DASHBOARD_VERSION_KEY);
    if(object.contains(JSON_MIN_NODE_VERSION_KEY))        setMigrateDataString(m_minNodeVersion, object, JSON_MIN_NODE_VERSION_KEY);
    if(object.contains(JSON_PUB_STAGE_KEY))               setMigrateDataString(m_pubStage, object, JSON_PUB_STAGE_KEY);
    if(object.contains(JSON_PENDING_SERIAL_KEY_KEY) && setMigrateDataString(m_pendingSerialKey, object, JSON_PENDING_SERIAL_KEY_KEY))
    {
        saveToSettings(TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
    }


    if(object.contains(JSON_SERIAL_KEY_DATA_KEY))
    {
        QString serialKey;
        jsonToValue(serialKey, object, JSON_SERIAL_KEY_KEY);
        bool isActivated;
        jsonToValue(isActivated, object, JSON_IS_ACTIVATED_KEY);
        QString serviceSerialKey = m_serialKeyData->serialKey();
        qint64 timeStemp = object[JSON_LISENSE_TIME_KEY].toString().toLongLong();
        QDateTime clientTime = QDateTime::fromSecsSinceEpoch(timeStemp);
        bool isDiffKey = serviceSerialKey != serialKey;
        bool clientKeyIsEmpty = serialKey.isEmpty();
        bool isServiceKeyIsEmpty = serviceSerialKey.isEmpty();

        if(isDiffKey &&
        (( !clientKeyIsEmpty && isServiceKeyIsEmpty) ||
        (!clientKeyIsEmpty && !isServiceKeyIsEmpty && isActivated && !m_serialKeyData->isActivated())||
        (!clientKeyIsEmpty && !isServiceKeyIsEmpty && isActivated && m_serialKeyData->isActivated() && m_serialKeyData->activatedDate() < clientTime)))
        {
            qInfo() << "The serialKey from the client differs from the serialKey in the service."
                       << " serviceData: " << serviceSerialKey << " clientData: " << serialKey;
            m_serialKeyData->setSerialKey(std::move(serialKey));
            m_serialKeyData->setActivated(isActivated);
            m_serialKeyData->setDateActivate(clientTime);
            saveSerialKeyData();
        }
    }

    if(object.contains(JSON_SERIAL_KEY_DATA_LIST_KEY))    setSerialKeyDataList(object[JSON_SERIAL_KEY_DATA_LIST_KEY].toArray());
    if(object.contains(JSON_BUG_REPORT_HISTORY_KEY))      setBugReportHistory(object[JSON_BUG_REPORT_HISTORY_KEY].toArray());
    if(object.contains(JSON_SERIAL_KEY_HISTORY_KEY))      setSerialKeyHistory(object[JSON_SERIAL_KEY_HISTORY_KEY].toArray());
}
