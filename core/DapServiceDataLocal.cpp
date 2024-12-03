#include "DapServiceDataLocal.h"
#include "DapSerialKeyData.h"
#include "DapSerialKeyHistory.h"
#include <QStandardPaths>

#ifdef Q_OS_WIN
#include "registry.h"
#endif

DapServiceDataLocal::DapServiceDataLocal()
{
#ifdef Q_OS_WIN
    QString path = QString("%1/%2/config/%2Service.conf").arg(regWGetUsrPath()).arg(DAP_BRAND);
    qDebug() << "[DapServiceDataLocal] Config path: " << path;
    initSettings(path);
    initData();
    initAuthData();
#else
    initSettings();
    initData();
    initAuthData();
#endif
}

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
    auto setMigrateData = [] (QString &data, const QString& value) -> bool
    {
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
    if(object.contains(JSON_KELVPN_PUB_KEY))            setMigrateData(m_kelvpnPub, object[JSON_KELVPN_PUB_KEY].toString());
    if(object.contains(JSON_NETWORK_DEFAULT_KEY))       setMigrateData(m_networkDefault, object[JSON_NETWORK_DEFAULT_KEY].toString());
    if(object.contains(JSON_URL_SITE_KEY))              setMigrateData(m_urlSite, object[JSON_URL_SITE_KEY].toString());
    if(object.contains(JSON_COUNTRY_ISO_KEY))           setMigrateData(m_coutryISO, object[JSON_COUNTRY_ISO_KEY].toString());
    if(object.contains(JSON_SETTINGS_KEY))              setSettings(object[JSON_SETTINGS_KEY].toObject());

    bool isNewLogin = false;
    bool isNewPassword = false;
    if(object.contains(JSON_LOGIN_KEY))
    {
        isNewLogin = setMigrateData(m_login, object[JSON_LOGIN_KEY].toString());
    }
    if(object.contains(JSON_PASSWORD_KEY))
    {
        isNewPassword = setMigrateData(m_password, object[JSON_PASSWORD_KEY].toString());
    }
    if(isNewLogin || isNewPassword)
    {
        saveAuthorizationData();
    }
    if(object.contains(JSON_MIN_DASHBOARD_VERSION_KEY))   setMigrateData(m_minDashboardVersion,  object[JSON_MIN_DASHBOARD_VERSION_KEY].toString());
    if(object.contains(JSON_MIN_NODE_VERSION_KEY))        setMigrateData(m_minNodeVersion,  object[JSON_MIN_NODE_VERSION_KEY].toString());
    if(object.contains(JSON_PUB_STAGE_KEY))               setMigrateData(m_pubStage,  object[JSON_PUB_STAGE_KEY].toString());
    if(object.contains(JSON_PENDING_SERIAL_KEY_KEY) && setMigrateData(m_pendingSerialKey, object[JSON_PENDING_SERIAL_KEY_KEY].toString()))
    {
        saveToSettings(TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
    }

    if(object.contains(JSON_SERIAL_KEY_DATA_KEY))
    {
        QJsonObject dataObject = object[JSON_SERIAL_KEY_DATA_KEY].toObject();
        QString serialKey;
        jsonToValue(serialKey, dataObject, JSON_SERIAL_KEY_KEY);
        bool isActivated;
        jsonToValue(isActivated, dataObject, JSON_IS_ACTIVATED_KEY);
        QString serviceSerialKey = m_serialKeyData->serialKey();
        qint64 timeStemp = dataObject[JSON_LISENSE_TIME_KEY].toString().toLongLong();
        QDateTime clientTime = QDateTime::fromSecsSinceEpoch(timeStemp);
        bool isDiffKey = serviceSerialKey != serialKey;
        bool clientKeyIsEmpty = serialKey.isEmpty();
        bool isServiceKeyIsEmpty = serviceSerialKey.isEmpty();

        if(isDiffKey && (( !clientKeyIsEmpty && isServiceKeyIsEmpty) ||
        ((!clientKeyIsEmpty && !isServiceKeyIsEmpty) &&
        ((isActivated && !m_serialKeyData->isActivated()) ||
        ((isActivated && m_serialKeyData->isActivated()) && (m_serialKeyData->activatedDate() < clientTime))))))
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
