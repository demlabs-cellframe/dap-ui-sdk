#include <QStandardPaths>

#include <QXmlStreamReader>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QCoreApplication>
#include <QTime>

#include "DapBaseDataLocal.h"
#include "DapSerialKeyData.h"
#include "DapSerialKeyHistory.h"

#include "DapCdbManager.h"

#include "dap_net.h"
#include <QJsonDocument>

#ifdef DAP_OS_ANDROID
#include "DapLogger.h"
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

const QString DapBaseDataLocal::TEXT_SERIAL_KEY           = "serialkey";
const QString DapBaseDataLocal::TEXT_SERIAL_KEY_HISTORY   = "serialkeyhistory";
const QString DapBaseDataLocal::TEXT_PENDING_SERIAL_KEY   = "pendingserialkey";
const QString DapBaseDataLocal::UPDATE_FILE_PATH          = "updatefilepath";
const QString DapBaseDataLocal::TEXT_BUGREPORT_HISTORY    = "bugreporthistory";
const QString DapBaseDataLocal::ROUTING_EXCEPTIONS_LIST   = "routingexceptionslist";
const QString DapBaseDataLocal::TEXT_LOGIN                = "login";
const QString DapBaseDataLocal::TEXT_PASSWORD             = "password";
const QString DapBaseDataLocal::TEXT_TX_OUT               = "tx_out";
const QString DapBaseDataLocal::COUNTRY_NAME              = "Country_code";
const QString DapBaseDataLocal::SETTING_AUTHORIZATION     = "authorization";
const QString DapBaseDataLocal::LAST_SELECTED_SERVER      = "lastSelectedServer";
const QString DapBaseDataLocal::LAST_SELECTED_SERVER_HASH = "lastSelectedServerOrderHash";
const QString DapBaseDataLocal::SETTING_THEME             = "ColorTheme";
const QString DapBaseDataLocal::SETTING_ROUEXC_MODE       = "rouExcMode";
const QString DapBaseDataLocal::NODE_ORDER_HISTORY        = "nodeOrderHistory";
const QString DapBaseDataLocal::SERIAL_LOSS_ON_UNINSTALL_FLAG = "serialLossOnUninstallFlag";
const QString DapBaseDataLocal::NOTIFICATION_HISTORY      = "notificationHistory";
const QString DapBaseDataLocal::NOCDB_DATA                = "NoCdbData";
const QString DapBaseDataLocal::COUNTRY_ISO               = "country_iso";
const QString DapBaseDataLocal::SETTING_LOCALE            = "language";
const QString DapBaseDataLocal::SETTING_SYS_LOCALE        = "SysLanguage";
const QString DapBaseDataLocal::AGREEMENT_CHECKED         = "agreementChecked";
const QString DapBaseDataLocal::LAST_NODE_LIST_UPDATE     = "last_nodelist_update";
const QString DapBaseDataLocal::LAST_NODE_LIST_UPDATE_TIME = "last_nodelist_update_time";
const QString DapBaseDataLocal::MIGRATION_KEY             = "migration";

DapBaseDataLocal::DapBaseDataLocal()
    : QObject()
    , m_serialKeyData(new DapSerialKeyData (this))
    , m_bugReportHistory(new DapBugReportHistory (this))
    , m_serialKeyHistory(new DapSerialKeyHistory (this))
{
    qDebug() << "[DL] DapBaseDataLocal Constructor";
    m_bugReportHistory->setSaveCallback([this]{ saveBugReport(); });
}

void DapBaseDataLocal::initData()
{
    // initSettings();
    parseXML(":/data.xml");
    initSecretKey();
}

void DapBaseDataLocal::initAuthData()
{
    loadAuthorizationDatas();
    loadKeysHistory();
    loadBugReport();
}

void DapBaseDataLocal::saveMigrate()
{
    DapBaseDataLocal::saveValueSetting(MIGRATION_KEY, true);
}

void DapBaseDataLocal::parseXML(const QString& a_fname)
{
    QFile file(a_fname);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Can't open data.xml from built in resource";
        return;
    }
    qDebug() << "data.xml opened, size "<< file.size();
    QXmlStreamReader *streamReader = new QXmlStreamReader(&file);
    if(streamReader->readNextStartElement())
    {
        if(streamReader->name().toString() == "data")
        {
            while ( streamReader->readNextStartElement() )
            {
                if( streamReader->name().toString() == "servers")
                {
                    while ( streamReader->readNextStartElement() )
                    {
                        if( streamReader->name().toString() == "server")
                        {
                            DapServerInfo item;
                            while ( streamReader->readNextStartElement() )
                            {
                                if(streamReader->name().toString() == "name")
                                {
                                    item.setName (streamReader->readElementText());
                                }
                                else if (streamReader->name().toString() == "address")
                                {
                                    item.setAddress (streamReader->readElementText());
                                }
                                else if( streamReader->name().toString() == "port")
                                {
                                    bool ok;
                                    quint16 port = quint16(streamReader->readElementText().toInt(&ok));
                                    if (!ok)
                                    {
                                        throw std::runtime_error("Can't cast port to int "
                                                                 "from XML file");
                                    }
                                    item.setPort (port);
                                }
                                else if(streamReader->name().toString() == "location")
                                {
                                    item.setLocation (streamReader->readElementText());
                                }
                                else if (streamReader->name().toString() == "state")
                                {
                                    item.setOnline (streamReader->readElementText());
                                }
                                else
                                {
                                    qWarning() << "[DL] Inside tag 'server': Unknown tag "<<streamReader->name();
                                    streamReader->skipCurrentElement();
                                }
                            }
                            qDebug() << "[DL] Server "<<item.name()<<" added";
                            DapServerList::instance()->append (std::move (item));
                        }
                        else
                        {
                            qDebug() << "[DL] Inside tag 'servers': unknown tag "<<streamReader->name();
                            streamReader->skipCurrentElement();
                        }
                    }
                }
                else if( streamReader->name().toString() == "cdb")
                {
                    QByteArray l_cdb_addr_qstr = streamReader->readElementText().toLatin1();
                    char* l_cdb_addr = l_cdb_addr_qstr.data();
                    char l_host[DAP_HOSTADDR_STRLEN + 1] = { '\0' };
                    uint16_t l_port = 0;

                    if (dap_net_parse_config_address(l_cdb_addr, l_host, &l_port, NULL, NULL)) {
                        QString addressToUse = QString(l_host);
                        uint16_t portToUse = l_port ? l_port : 80;

                        struct sockaddr_storage l_addr_out = {};
                        if (dap_net_resolve_host(l_host, dap_itoa(portToUse), false, &l_addr_out, NULL)) {
                            char l_addr_out_str[NI_MAXHOST] = {0};
                            char servInfo[NI_MAXSERV];
                            if (!getnameinfo((struct sockaddr*)&l_addr_out, sizeof(l_addr_out),
                                             l_addr_out_str, NI_MAXHOST,
                                             servInfo, NI_MAXSERV,
                                             NI_NUMERICHOST | NI_NUMERICSERV)) {
                                addressToUse = QString(l_addr_out_str);
                            }
                        }

                        DapCdbManager::instance().addServer(addressToUse, portToUse);
                    }

                }
                else if( streamReader->name().toString() == "network-default")
                {
                    m_networkDefault = streamReader->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }
                else if( streamReader->name().toString() == "kelvpn-pub")
                {
                    struct sockaddr_storage l_addr_out = {};
                    QByteArray l_pub_addr_qstr = streamReader->readElementText().toLatin1();
                    char *l_pub_addr = l_pub_addr_qstr.data();
                    char l_host[DAP_HOSTADDR_STRLEN + 1] = { '\0' }; uint16_t l_port = 0;
                    if(dap_net_parse_config_address(l_pub_addr, l_host, &l_port, NULL, NULL)
                        && dap_net_resolve_host(l_host, dap_itoa(l_port), false, &l_addr_out, NULL)){
                        char l_addr_out_str[NI_MAXHOST] = {0};
                        char servInfo[NI_MAXSERV];
                        if (!getnameinfo((struct sockaddr*)&l_addr_out, sizeof(l_addr_out), l_addr_out_str, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV)){
                            m_kelvpnPub = QString(l_addr_out_str);
                            qInfo() << "Pub address: " << m_kelvpnPub;
                        }
                    }
                }
                else if( streamReader->name().toString() == "min-node-version")
                {
                    m_minNodeVersion = streamReader->readElementText();
                    qInfo() << "Min node version: " << m_minNodeVersion;
                }
                else if( streamReader->name().toString() == "min-dashboard-version")
                {
                    m_minDashboardVersion = streamReader->readElementText();
                    qInfo() << "Min dashboard version: " << m_minDashboardVersion;
                }
                else if( streamReader->name().toString() == "title")
                {
                    m_brandName = streamReader->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }
                else if( streamReader->name().toString() == "url_site")
                {
                    m_urlSite = streamReader->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }
                else
                {
                    qDebug() << "[DL] Inside tag 'data' unknown tag "<<streamReader->name();
                    streamReader->skipCurrentElement();
                }
            }
        }
    }
    file.close();
#ifdef  QT_DEBUG
    DapServerList::instance()->append (DapServerInfo {"UNKNOWN", "local", "127.0.0.1",  8099});
#endif
    delete streamReader;
}

/// Get login.
/// @return Login.
QString DapBaseDataLocal::login() const
{
    return m_login;
}

/// Set login.
/// @param login Login.
void DapBaseDataLocal::setLogin(const QString &a_login)
{
    if (m_login == a_login)
        return;
    m_login = a_login;

    emit loginChanged(m_login);
}

/// Get password.
/// @return Password.
QString DapBaseDataLocal::password() const
{
    return m_password;
}

/// Set password.
/// @param password Password.
void DapBaseDataLocal::setPassword(const QString &a_password)
{
    if (m_password == a_password)
        return;
    m_password = a_password;

    emit passwordChanged(m_password);
}

void DapBaseDataLocal::saveAuthorizationData()
{
    saveEncryptedSetting(TEXT_LOGIN     , this->login());
    saveEncryptedSetting(TEXT_PASSWORD  , this->password());
}

void DapBaseDataLocal::saveSerialKeyData()
{
    if (m_serialKeyData)
    {
        saveToSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
    }
}

void DapBaseDataLocal::resetSerialKeyData()
{
    qDebug() << "Resetting serial key data...";

    if (m_serialKeyData)
    {
        qDebug() << "Serial key data exists, performing reset.";
        m_serialKeyData->reset();
        this->saveToSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
        qDebug() << "Serial key data has been reset and saved to settings.";
    }
    else
    {
        qDebug() << "No serial key data to reset.";
    }
}

void DapBaseDataLocal::savePendingSerialKey(QString a_serialkey)
{
    m_pendingSerialKey = a_serialkey;
    this->saveToSettings(TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
}

void DapBaseDataLocal::loadAuthorizationDatas()
{
#ifdef Q_OS_ANDROID
    auto keys = settings()->allKeys();
    if (keys.isEmpty()) {
        QSettings oldSettings;
        for (auto key : oldSettings.allKeys()) {
            settings()->setValue(key, oldSettings.value(key));
        }
    }
#endif

    this->setLogin(getEncryptedSetting(TEXT_LOGIN).toString());
    this->setPassword(getEncryptedSetting(TEXT_PASSWORD).toString());

    if (m_serialKeyData)
        this->loadFromSettingsBase(TEXT_SERIAL_KEY, *m_serialKeyData);
    this->loadFromSettingsBase(TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
}

void DapBaseDataLocal::initSettings(const QString& path)
{
#ifdef Q_OS_ANDROID
    QString s_path = DapLogger::defaultLogPath(DAP_BRAND).chopped(3).append("settings.ini");

    /* Legacy settings import, will be deprecated on targeting API 30+ */
    int l_ofd = open(qPrintable(s_path), O_RDONLY);
    if (l_ofd <= 0) {
        int _l_fd = open("/sdcard/KelvinVPN/log/settings.ini", O_RDWR);
        int l_fd = _l_fd > 0 ? _l_fd : open("/sdcard/" DAP_BRAND "/log/settings.ini", O_RDWR);
        if (l_fd > 0) {
            int l_ofd = open(qPrintable(s_path), O_CREAT | O_RDWR);
            struct stat statBuf;
            fstat(l_fd, &statBuf);
            qInfo() << "Imported old settings [" << sendfile(l_ofd, l_fd, NULL, statBuf.st_size) << "] bytes";
            ftruncate(l_fd, 0);
            close(l_fd);
            close(l_ofd);
        } else {
            qInfo() << "Old settings not found";
        }
    } else {
        close(l_ofd);
    }

    m_settings = new QSettings(s_path, QSettings::IniFormat);
#else
    if(path.isEmpty()){
        m_settings = new QSettings();
    }
    else {
        m_settings = new QSettings(path, QSettings::IniFormat);
    }
#endif
}

QVariant DapBaseDataLocal::getEncryptedSetting(const QString &a_setting)
{
    QByteArray outString;
    this->loadEncryptedSettingString(a_setting, outString);
    return QString(outString);
}

bool DapBaseDataLocal::loadEncryptedSettingString(const QString &a_setting, QByteArray& a_outString)
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
    secretKey->decode(encryptedString, a_outString);

    return true;
}

void DapBaseDataLocal::saveEncryptedSetting(const QString &a_setting, const QVariant &a_value)
{
    this->saveEncryptedSetting(a_setting, a_value.toByteArray());
}

void DapBaseDataLocal::saveEncryptedSetting(const QString &a_setting, const QByteArray &a_string)
{
    QByteArray encodedString;
    secretKey->encode(a_string, encodedString);
    saveValueSetting(a_setting, encodedString);
}

QVariant DapBaseDataLocal::getValueSetting(const QString& a_setting)
{
    return settings()->value(a_setting);
}

void DapBaseDataLocal::saveValueSetting(const QString& a_setting, const QVariant& a_value)
{
    settings()->setValue(a_setting, a_value);
    settings()->sync();
}

void DapBaseDataLocal::removeValueSetting(const QString& a_setting)
{
    settings()->remove(a_setting);
}

void DapBaseDataLocal::setSettings(const QJsonObject &json)
{
    QStringList keys = json.keys();
    qDebug() << "[DapBaseDataLocal][setSettings] New value of the settings keys - " << keys;
    for(const auto& key: json.keys())
    {
        if(key == "remove")
        {
            m_settings->remove(json[key].toString());
        }
        else
        {
            if(key == ROUTING_EXCEPTIONS_LIST)
            {
                saveValueSetting(key, json[key].toObject());
            }
            else if(key == NOTIFICATION_HISTORY || key == NODE_ORDER_HISTORY)
            {
                auto jsonDoc = QJsonDocument::fromJson(json[key].toString().toUtf8());
                QByteArray result = QJsonDocument (jsonDoc).toJson (QJsonDocument::JsonFormat::Compact);
                saveToSettings(key, result);
            }
            else
            {
                m_settings->setValue(key, json[key].toVariant());
            }
        }
    }
}

DapSerialKeyData *DapBaseDataLocal::serialKeyData()
{
    return m_serialKeyData;
}

DapBugReportHistory * DapBaseDataLocal::bugReportHistory()
{
    return m_bugReportHistory;
}

DapSerialKeyHistory *DapBaseDataLocal::serialKeyHistory()
{
    return m_serialKeyHistory;
}

void DapBaseDataLocal::initSecretKey()
{
    if (settings()->value("key").toString().isEmpty())
    {
        settings()->setValue("key", getRandomString(40));
    }
    if (secretKey != nullptr) {
        delete secretKey;
    }
    secretKey = new DapKey(DAP_ENC_KEY_TYPE_IAES, settings()->value("key").toString() + "SLKJGN234njg6vlkkNS3s5dfzkK5O54jhug3KUifw23");
}

QString DapBaseDataLocal::getRandomString(int size)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = size;

    QString randomString;
    for(int i=0; i < randomStringLength; ++i){
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

Authorization DapBaseDataLocal::authorizationType()
{
    auto auth = getValueSetting(SETTING_AUTHORIZATION).toString();
    if (auth == "serialKey")
        return Authorization::serialKey;
    if (auth == "account")
        return Authorization::account;
    if (auth == "certificate")
        return Authorization::certificate;
#ifdef BRAND_RISEVPN
    // default value
    return Authorization::undefined;
#else
    // default value
    return Authorization::account;
#endif
}

void DapBaseDataLocal::setAuthorizationType(Authorization type)
{
    QString auth;
    if (type == Authorization::serialKey)
        auth = "serialKey";
    if (type == Authorization::account)
        auth = "account";
    if (type == Authorization::certificate)
        auth = "certificate";
    if (type == Authorization::undefined)
        auth = "";
    saveValueSetting(SETTING_AUTHORIZATION, auth);
}

void DapBaseDataLocal::loadKeysHistory()
{
    QStringList list;
    loadFromSettingsBase(TEXT_SERIAL_KEY_HISTORY, list);
    m_serialKeyHistory->setKeyList(std::move(list));
}

void DapBaseDataLocal::saveKeysHistory()
{
    auto list = serialKeyHistory()->getKeysHistory();
    saveToSettings (TEXT_SERIAL_KEY_HISTORY, list);
}

void DapBaseDataLocal::addNewSerialKey()
{
    auto key  = serialKeyData()->serialKey();
    if (serialKeyHistory()->getKeysHistory().contains(key))
    {
        return;
    }
    serialKeyHistory()->appendKey(key);
    saveKeysHistory();
    emit serialKeyAdded();
}

void DapBaseDataLocal::loadBugReport()
{
    if (!m_bugReportHistory->getBagreports().isEmpty())
        return m_bugReportHistory->bagreportListLoaded();

    QMap<int, DapBugReportHistoryItem> result;

    QJsonArray array;

    if (loadFromSettingsBase(TEXT_BUGREPORT_HISTORY, array)
        && !array.isEmpty())
    {
        /* parse array */
        for (const auto &obj : qAsConst (array))
        {
            auto item = m_bugReportHistory->jsonToItem(obj.toObject());
            if (item.number)
                result.insert (item.number, std::move (item));
        }
    }

    m_bugReportHistory->setBugReports(std::move(result));
}

void DapBaseDataLocal::saveBugReport()
{
    QJsonArray array;

    for (const auto &item : qAsConst(m_bugReportHistory->getBagreports()))
        array << m_bugReportHistory->itemToJson(item);

    saveToSettings (TEXT_BUGREPORT_HISTORY, array);
    qDebug() << "[DapBaseDataLocal] BugReport saved";
}

QJsonObject DapBaseDataLocal::toJson()
{
    QJsonObject resultObject;

    resultObject.insert(JSON_KELVPN_PUB_KEY, m_kelvpnPub);
    resultObject.insert(JSON_NETWORK_DEFAULT_KEY, m_networkDefault);
    resultObject.insert(JSON_URL_SITE_KEY, m_urlSite);
    resultObject.insert(JSON_COUNTRY_ISO_KEY, m_coutryISO);
    resultObject.insert(JSON_LOGIN_KEY, m_login);
    resultObject.insert(JSON_PASSWORD_KEY, m_password);
    resultObject.insert(JSON_MIN_DASHBOARD_VERSION_KEY, m_minDashboardVersion);
    resultObject.insert(JSON_MIN_NODE_VERSION_KEY, m_minNodeVersion);
    resultObject.insert(JSON_PUB_STAGE_KEY, m_pubStage);
    resultObject.insert(JSON_PENDING_SERIAL_KEY_KEY, m_pendingSerialKey);

    resultObject.insert(JSON_DATA_TO_UPDATE_KEY, dataToUpdateToJson());
    resultObject.insert(JSON_SERIAL_KEY_DATA_KEY, serialKeyDataToJson());
    resultObject.insert(JSON_SERIAL_KEY_DATA_LIST_KEY, serialKeyDataListToJson());
    resultObject.insert(JSON_BUG_REPORT_HISTORY_KEY, bugReportHistoryToJson());
    resultObject.insert(JSON_SERIAL_KEY_HISTORY_KEY, serialKeyHistoryToJson());
    resultObject.insert(JSON_SETTINGS_KEY, settingsToJson());

    // Check if the resulting JSON is too large (MAX_BUFFER_SIZE = 10000 bytes)
    const int MAX_BUFFER_SIZE = 10000;
    QByteArray jsonData = QJsonDocument(resultObject).toJson(QJsonDocument::JsonFormat::Compact);
    
    if (jsonData.size() > MAX_BUFFER_SIZE)
    {
        qWarning() << "[DapBaseDataLocal][toJson] Generated JSON is too large (" 
                   << jsonData.size() << " bytes), exceeds MAX_BUFFER_SIZE (" 
                   << MAX_BUFFER_SIZE << " bytes)";
        
        // Try to reduce the size by removing less critical data
        // First, try to reduce settings further
        QJsonObject reducedSettings = settingsToJson();
        
        // If settings contain notification history, try to reduce it more aggressively
        if (m_settings && m_settings->allKeys().contains(NOTIFICATION_HISTORY))
        {
            qDebug() << "[DapBaseDataLocal][toJson] Attempting to reduce notification history size";
            
            QByteArray source;
            loadFromSettingsBase(NOTIFICATION_HISTORY, source);
            QJsonArray notifications = QJsonDocument::fromJson(source).array();
            
            // More aggressive reduction: limit to 20 entries
            const int REDUCED_MAX_NOTIFICATIONS = 20;
            QJsonArray reducedNotifications;
            
            QDate threeDaysAgo = QDate::currentDate().addDays(-3);
            int count = 0;
            
            for (const auto &item : qAsConst(notifications))
            {
                if (count >= REDUCED_MAX_NOTIFICATIONS) break;
                
                QJsonObject notification = item.toObject();
                bool isTitle = notification.value("isTitle").toBool();
                
                bool includeItem = false;
                if (isTitle)
                {
                    QString dateString = notification.value("date").toString();
                    if (!dateString.isEmpty())
                    {
                        QDate titleDate = QDate::fromString(dateString, "dd.MM.yyyy");
                        includeItem = titleDate.isValid() && titleDate >= threeDaysAgo;
                    }
                }
                else
                {
                    QString createdString = notification.value("created").toString();
                    if (!createdString.isEmpty())
                    {
                        QDateTime createdDateTime = QDateTime::fromString(createdString, "hh:mm:ss dd.MM.yyyy");
                        includeItem = createdDateTime.isValid() && createdDateTime.date() >= threeDaysAgo;
                    }
                }
                
                if (includeItem)
                {
                    reducedNotifications.append(notification);
                    count++;
                }
            }
            
            QByteArray reducedNotificationData = QJsonDocument(reducedNotifications).toJson(QJsonDocument::JsonFormat::Compact);
            QJsonValue reducedJsonValue = QJsonValue::fromVariant(QVariant(reducedNotificationData));
            reducedSettings.insert(NOTIFICATION_HISTORY, reducedJsonValue);
            
            resultObject.insert(JSON_SETTINGS_KEY, reducedSettings);
            
            // Check size again
            jsonData = QJsonDocument(resultObject).toJson(QJsonDocument::JsonFormat::Compact);
            qDebug() << "[DapBaseDataLocal][toJson] After notification reduction, JSON size:" << jsonData.size() << "bytes";
        }
        
        // If still too large, remove less critical components
        if (jsonData.size() > MAX_BUFFER_SIZE)
        {
            qWarning() << "[DapBaseDataLocal][toJson] JSON still too large after notification reduction, removing less critical data";
            
            // Remove serial key history as it's less critical
            resultObject.remove(JSON_SERIAL_KEY_HISTORY_KEY);
            
            // Remove bug report history if still too large
            jsonData = QJsonDocument(resultObject).toJson(QJsonDocument::JsonFormat::Compact);
            if (jsonData.size() > MAX_BUFFER_SIZE)
            {
                resultObject.remove(JSON_BUG_REPORT_HISTORY_KEY);
                qDebug() << "[DapBaseDataLocal][toJson] Removed bug report history, final size:" 
                         << QJsonDocument(resultObject).toJson(QJsonDocument::JsonFormat::Compact).size() << "bytes";
            }
        }
    }
    else
    {
        qDebug() << "[DapBaseDataLocal][toJson] JSON size:" << jsonData.size() << "bytes (within limits)";
    }

    return resultObject;
}

void DapBaseDataLocal::fromJson(const QJsonObject &json)
{
    if(json.contains(JSON_SETTINGS_KEY))                setSettings(json[JSON_SETTINGS_KEY].toObject());
    if(json.contains(JSON_MIN_DASHBOARD_VERSION_KEY))   jsonToValue(m_minDashboardVersion, json, JSON_MIN_DASHBOARD_VERSION_KEY);
    if(json.contains(JSON_MIN_NODE_VERSION_KEY))        jsonToValue(m_minNodeVersion, json, JSON_MIN_NODE_VERSION_KEY);
    if(json.contains(JSON_PUB_STAGE_KEY))               jsonToValue(m_pubStage, json, JSON_PUB_STAGE_KEY);
    if(json.contains(JSON_PENDING_SERIAL_KEY_KEY))      jsonToValue(m_pendingSerialKey, json, JSON_PENDING_SERIAL_KEY_KEY);
    if(json.contains(JSON_KELVPN_PUB_KEY))              jsonToValue(m_kelvpnPub, json, JSON_KELVPN_PUB_KEY);
    if(json.contains(JSON_NETWORK_DEFAULT_KEY))         jsonToValue(m_networkDefault, json, JSON_NETWORK_DEFAULT_KEY);
    if(json.contains(JSON_URL_SITE_KEY))                jsonToValue(m_urlSite, json, JSON_URL_SITE_KEY);
    if(json.contains(JSON_COUNTRY_ISO_KEY))             jsonToValue(m_coutryISO, json, JSON_COUNTRY_ISO_KEY);
    if(json.contains(JSON_LOGIN_KEY))
    {
        jsonToValue(m_login, json, JSON_LOGIN_KEY);
        emit loginChanged(m_login);
    }
    if(json.contains(JSON_PASSWORD_KEY))
    {
        jsonToValue(m_password, json, JSON_PASSWORD_KEY);
        emit passwordChanged(m_password);
    }
    if(json.contains(JSON_SERIAL_KEY_DATA_KEY))         setSerialKeyData(json[JSON_SERIAL_KEY_DATA_KEY].toObject());

    if(json.contains(JSON_DATA_TO_UPDATE_KEY))          setDataToUpdate(json[JSON_DATA_TO_UPDATE_KEY].toObject());
    if(json.contains(JSON_SERIAL_KEY_DATA_LIST_KEY))    setSerialKeyDataList(json[JSON_SERIAL_KEY_DATA_LIST_KEY].toArray());
    if(json.contains(JSON_BUG_REPORT_HISTORY_KEY))      setBugReportHistory(json[JSON_BUG_REPORT_HISTORY_KEY].toArray());
    if(json.contains(JSON_SERIAL_KEY_HISTORY_KEY))      setSerialKeyHistory(json[JSON_SERIAL_KEY_HISTORY_KEY].toArray());
}

void DapBaseDataLocal::jsonToValue(QString& data, const QJsonObject& object, const QString& key)
{
    if(!object.contains(key))
    {
        for (const QString& outerKey : object.keys())
        {
            const QJsonValue& outerValue = object[outerKey];
            if (outerValue.isObject())
            {
                jsonToValue(data, outerValue.toObject(), key);
                if (!data.isEmpty())
                {
                    return;
                }
            }
        }
    }
    else
    {
        QString value = object[key].toString();
        if (value == data) 
        {
            return;
        }
        data = value;
    }
}

void DapBaseDataLocal::jsonToValue(bool& data, const QJsonObject& object, const QString& key)
{
    if(!object.contains(key))
    {
        for (const QString& outerKey : object.keys())
        {
            const QJsonValue& outerValue = object[outerKey];
            if (outerValue.isObject())
            {
                jsonToValue(data, outerValue.toObject(), key);
                if (data)
                {
                    return;
                }
            }
        }
    }
    else
    {
        bool value = object[key].toBool();
        if (value == data) 
        {
            return;
        }
        data = value;
    }
}

void DapBaseDataLocal::jsonToValue(int& data, const QJsonObject& object, const QString& key)
{
    if (object.contains(key))
    {
        for (const QString& outerKey : object.keys())
        {
            const QJsonValue& outerValue = object[outerKey];
            if (outerValue.isObject())
            {
                jsonToValue(data, outerValue.toObject(), key);
                if (data != 0)
                {
                    return;
                }
            }
        }
    }
    else
    {
        int value = object[key].toInt();
        if (value == data) 
        {
            return;
        }
        data = value;
    }
}

QJsonObject DapBaseDataLocal::createJsonObject(const QString& itemName, const QString& itemValue)
{
    QJsonObject result;
    result.insert(itemName, itemValue);
    return result;
}

const QJsonObject DapBaseDataLocal::dataToUpdateToJson() const
{
    QJsonObject dataToUpdate;
    dataToUpdate.insert(JSON_STATE_KEY, m_dataToUpdate.state == DownloadingUpdateState::Default ? "Default" : "Download");
    dataToUpdate.insert(JSON_VERSION_KEY, m_dataToUpdate.version);
    dataToUpdate.insert(JSON_URL_UPDATE_KEY, m_dataToUpdate.URLUpdate);
    dataToUpdate.insert(JSON_SHOW_MESSAGE_KEY, m_dataToUpdate.showMessage);
    dataToUpdate.insert(JSON_FORCE_DOWNLOAD_KEY, m_dataToUpdate.forceDownload);
    dataToUpdate.insert(JSON_FORCE_INSTALL_KEY, m_dataToUpdate.forceInstall);
    return dataToUpdate;
}

void DapBaseDataLocal::setDataToUpdate(const QJsonObject& object)
{
    QString state;
    jsonToValue(state, object, JSON_STATE_KEY);
    m_dataToUpdate.state = state == "Default" ? DownloadingUpdateState::Default : DownloadingUpdateState::Download;
    jsonToValue(m_dataToUpdate.version, object, JSON_VERSION_KEY);
    jsonToValue(m_dataToUpdate.URLUpdate, object, JSON_URL_UPDATE_KEY);
    jsonToValue(m_dataToUpdate.showMessage, object, JSON_SHOW_MESSAGE_KEY);
    jsonToValue(m_dataToUpdate.forceDownload, object, JSON_FORCE_DOWNLOAD_KEY);
    jsonToValue(m_dataToUpdate.forceInstall, object, JSON_FORCE_INSTALL_KEY);
}

const QJsonObject DapBaseDataLocal::serialKeyDataToJson() const
{
    QJsonObject serialKeyData;
    serialKeyData.insert(JSON_SERIAL_KEY_KEY, m_serialKeyData->serialKey());
    serialKeyData.insert(JSON_IS_ACTIVATED_KEY, m_serialKeyData->isActivated());
    auto timeLicense = m_serialKeyData->licenseTermTill();
    qint64 timeStemp = timeLicense.toSecsSinceEpoch();
    serialKeyData.insert(JSON_LISENSE_TIME_KEY, QString::number(timeStemp));
    serialKeyData.insert(JSON_LICENSE_TERM_TILL_KEY, m_serialKeyData->licenseTermTill().toSecsSinceEpoch());
    return serialKeyData;
}

void DapBaseDataLocal::setSerialKeyData(const QJsonObject& object)
{
    if(object.isEmpty())
    {
        return;
    }
    QString serialKey = object.value(JSON_SERIAL_KEY_KEY).toString();
    bool hasSerial = object.contains(JSON_SERIAL_KEY_KEY);
    bool hasIsActivated = object.contains(JSON_IS_ACTIVATED_KEY);
    bool isActivated = object.value(JSON_IS_ACTIVATED_KEY).toBool();
    qint64 timeStemp = object.value(JSON_LISENSE_TIME_KEY).toString().toLongLong();
    qint64 licenseTermTillLong = object.value(JSON_LICENSE_TERM_TILL_KEY).toInt();
    QDateTime licenseTermTill = QDateTime::fromSecsSinceEpoch(licenseTermTillLong);
    QDateTime time = QDateTime::fromSecsSinceEpoch(timeStemp);

    // Only update serial if the incoming value is non-empty. This avoids
    // unintentionally clearing a valid stored serial key due to partial updates.
    if (hasSerial && !serialKey.isEmpty()) {
        m_serialKeyData->setSerialKey(std::move(serialKey));
    }

    // Update activation state if provided.
    if (hasIsActivated) {
        m_serialKeyData->setActivated(isActivated);
    }

    // Update activation date if provided (non-zero timestamp)
    if (timeStemp != 0) {
        m_serialKeyData->setDateActivate(time);
    }

    // Update license term if provided (non-zero)
    if (licenseTermTillLong != 0) {
        m_serialKeyData->setLicenseTermTill(licenseTermTill);
    }
}

const QJsonArray DapBaseDataLocal::serialKeyDataListToJson() const
{
    QJsonArray serialKeyDataList;
    for(const auto& item: m_serialKeyDataList)
    {
        serialKeyDataList.append(std::move(item));
    }
    return serialKeyDataList;
}

void DapBaseDataLocal::setSerialKeyDataList(const QJsonArray& list)
{
    for(const auto& item: list)
    {
        m_serialKeyDataList.insert(item.toString());
    }
}

const QJsonArray DapBaseDataLocal::bugReportHistoryToJson() const
{
    QJsonArray bugReportHistory;
    const auto& bugReports  = m_bugReportHistory->getBagreports();

    QList<int> keys = bugReports.keys();
    qDebug() << "[DapBaseDataLocal][bugReportHistoryToJson] the number of bug reports being moved: " << keys.size();

    for(const auto& key: qAsConst(keys))
    {
        QJsonObject object;
        const auto& item = bugReports[key];
        object.insert(JSON_BUG_NUMBER_KEY, key);
        object.insert(JSON_NUMBER_KEY, item.number);
        object.insert(JSON_STATUS_KEY, item.status);
        bugReportHistory.append(std::move(object));
    }
    return bugReportHistory;
}

void DapBaseDataLocal::setBugReportHistory(const QJsonArray& list)
{
    QMap<int, DapBugReportHistoryItem> bugReports;
    for(const auto& itemValue: list)
    {
        QJsonObject item = itemValue.toObject();
        if(!item.contains(JSON_NUMBER_KEY) || !item.contains(JSON_STATUS_KEY))
        {
            qWarning() << "[DapBaseDataLocal][fromJson] Error receiving bugReportHistory data";
            continue;
        }
        int bugNumber = item[JSON_BUG_NUMBER_KEY].toInt();
        DapBugReportHistoryItem historyItem{0, ""};
        historyItem.number = item[JSON_NUMBER_KEY].toInt();
        historyItem.status = item[JSON_STATUS_KEY].toString();
        bugReports.insert(bugNumber, std::move(historyItem));
    }
    m_bugReportHistory->setBugReports(std::move(bugReports));
}

const QJsonArray DapBaseDataLocal::serialKeyHistoryToJson() const
{
    QJsonArray serialKeyHistory;
    QStringList listKeys = m_serialKeyHistory->getKeysHistory();
    qDebug() << "[DapBaseDataLocal][serialKeyHistoryToJson] list of keys: " << listKeys;
    for(const auto& item: listKeys)
    {
        serialKeyHistory.append(std::move(item));
    }
    return serialKeyHistory;
}

void DapBaseDataLocal::setSerialKeyHistory(const QJsonArray& list)
{
    for(const auto& itemValue: list)
    {
        m_serialKeyHistory->appendKey(itemValue.toString());
    }
    if(!list.isEmpty())
    {
        saveKeysHistory();
    }
}

const QJsonObject DapBaseDataLocal::settingsToJson()
{
    QJsonObject settings;
    if(m_settings)
    {
        QStringList listKeys = m_settings->allKeys();
        qDebug() << "[DapBaseDataLocal][settingsToJson] setting keys: " << listKeys;
        for(const auto& key: qAsConst(listKeys))
        {
            if(TEXT_SERIAL_KEY == key || TEXT_PENDING_SERIAL_KEY == key || TEXT_SERIAL_KEY_HISTORY == key ||
                TEXT_BUGREPORT_HISTORY == key || LAST_NODE_LIST_UPDATE == key || LAST_NODE_LIST_UPDATE_TIME == key ||
                SETTING_THEME == key)
            {
                continue;
            }
            else if(key == NOTIFICATION_HISTORY)
            {
                QByteArray source;
                loadFromSettingsBase(key, source);
                
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
                        qDebug() << "[DapBaseDataLocal][settingsToJson] Notification history truncated to" << MAX_NOTIFICATIONS << "entries";
                        break;
                    }
                }
                
                // Convert filtered notifications back to compact JSON
                QByteArray filteredResult = QJsonDocument(filteredNotifications).toJson(QJsonDocument::JsonFormat::Compact);
                
                // Check if the result is too large and truncate further if needed
                const int MAX_NOTIFICATION_JSON_SIZE = 8000; // Leave some room for other settings
                if (filteredResult.size() > MAX_NOTIFICATION_JSON_SIZE)
                {
                    qWarning() << "[DapBaseDataLocal][settingsToJson] Notification history JSON too large (" 
                               << filteredResult.size() << " bytes), truncating further";
                    
                    // Remove entries from the end until size is acceptable
                    while (filteredNotifications.size() > 0 && filteredResult.size() > MAX_NOTIFICATION_JSON_SIZE)
                    {
                        filteredNotifications.removeLast();
                        filteredResult = QJsonDocument(filteredNotifications).toJson(QJsonDocument::JsonFormat::Compact);
                    }
                    
                    qDebug() << "[DapBaseDataLocal][settingsToJson] Notification history truncated to" 
                             << filteredNotifications.size() << "entries, size:" << filteredResult.size() << "bytes";
                }
                
                QJsonValue jsonValue = QJsonValue::fromVariant(QVariant(filteredResult));
                settings.insert(key, jsonValue);
            }
            else if(key == NODE_ORDER_HISTORY)
            {
                QByteArray source;
                loadFromSettingsBase(key, source);
                QJsonValue jsonValue = QJsonValue::fromVariant(QVariant(source));
                settings.insert(key, jsonValue);
            }
            else if(key == ROUTING_EXCEPTIONS_LIST)
            {
                auto obj = getValueSetting(key).toJsonObject();
                settings.insert(key, obj);
            }
            else
            {
                QVariant value = m_settings->value(key);
                settings.insert(key, QJsonValue::fromVariant(value));
            }
        }
    }
    return settings;
}

//-----------------------------------------------------------------------------------------------------------------------------------
