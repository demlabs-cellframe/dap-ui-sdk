#include <QStandardPaths>

#include <QXmlStreamReader>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QCoreApplication>
#include <algorithm>
#include <QTime>

#include "DapDataLocal.h"
#include "DapSerialKeyData.h"
#include "DapLogger.h"

#ifdef DAP_OS_ANDROID
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif



/// ConfigData

void ConfigData::parseXML(const QString& a_fname)
{
    QFile file(a_fname);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Can't open data.xml from built in resource";
        return;
    }
    qDebug() << "data.xml opened, size "<< file.size();
    QXmlStreamReader *sr = new QXmlStreamReader(&file);
    if(sr->readNextStartElement()){
        if(sr->name() == "data"){
            while ( sr->readNextStartElement() ){
                if( sr->name() == "servers"){
                    while ( sr->readNextStartElement() ){
                        if( sr->name() == "server") {
                            DapServerInfo item;
                            while ( sr->readNextStartElement() ){
                                if(sr->name() == "name"){
                                    item.name=sr->readElementText();
                                } else if (sr->name() == "address") {
                                    item.address=sr->readElementText();
                                } else if( sr->name() == "port") {
                                    bool ok;
                                    quint16 port = quint16(sr->readElementText().toInt(&ok));
                                    if (!ok) {
                                        throw std::runtime_error("Can't cast port to int "
                                                                 "from XML file");
                                    }
                                    item.port = port;
                                } else if(sr->name() == "location") {
                                    item.location = sr->readElementText();
                                } else if (sr->name() == "state") {
                                    item.online = sr->readElementText();
                                }
                                else {
                                    qWarning() << "[DL] Inside tag 'server': Unknown tag "<<sr->name();
                                    sr->skipCurrentElement();
                                }
                            }
                            qDebug() << "[DL] Server "<<item.name<<" added";
                            DapDataLocal::serversData()->addServer(item);
                        }else{
                            qDebug() << "[DL] Inside tag 'servers': unknown tag "<<sr->name();
                            sr->skipCurrentElement();
                        }
                    }
                }else if( sr->name() == "cdb"){
                    m_cdbServersList.push_back(sr->readElementText());
                    qInfo() << "Add CDB address: " << m_cdbServersList.back();
                }else if( sr->name() == "network-default"){
                    m_networkDefault = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }else if( sr->name() == "title"){
                    m_brandName = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }else if( sr->name() == "url_site"){
                    m_urlSite = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }else{
                    qDebug() << "[DL] Inside tag 'data' unknown tag "<<sr->name();
                    sr->skipCurrentElement();
                }
            }
        }
    }
    file.close();
#ifdef  QT_DEBUG
    DapDataLocal::serversData()->addServer("UNKNOWN", "local", "127.0.0.1",  8099);
#endif

    delete sr;
}

QJsonObject ConfigData::toJson()
{
    QJsonObject xmlData;
    QJsonArray cdbServersList;
    foreach (QString server,  m_cdbServersList)
        cdbServersList.push_back(server);
    xmlData["cdb"] = cdbServersList;
    xmlData["network-default"] = m_networkDefault;
    xmlData["title"] = m_brandName;
    xmlData["url_site"] = m_urlSite;
    return xmlData;
}

void ConfigData::fromJson(QJsonObject* data)
{
    return;
    foreach(QString key, data->keys())
    {
        if (key == "cdb" && data->value(key) !=QJsonValue::Undefined)
        {
            m_cdbServersList.clear();
            foreach(QJsonValue server, data->value(key).toArray())
                m_cdbServersList.append(server.toString());
            continue;
        }
        if (key == "network-default" && data->value(key) !=QJsonValue::Undefined)
        {
            m_networkDefault = data->value(key).toString();
            continue;
        }
        if (key == "title" && data->value(key) !=QJsonValue::Undefined)
        {
            m_brandName = data->value(key).toString();
            continue;
        }
        if (key == "url_site" && data->value(key) !=QJsonValue::Undefined)
        {
            m_urlSite = data->value(key).toString();
            continue;
        }
    }
}


/// DapDataSettings

QJsonObject DapDataSettings::toJson(QMap<QString, QVariant> a_data)
{
    QJsonObject settingData;
    foreach(const QString& key, a_data.keys())
        settingData[key] = DapDataSettings::packItem(a_data[key]);
    return settingData;
}

QJsonArray DapDataSettings::toJson(QStringList a_keys)
{
    QJsonArray settingKeys;
    foreach(const QString& key, a_keys)
        settingKeys.append(key);
    return settingKeys;
}

QJsonValue DapDataSettings::packItem(const QVariant& data)
{
    QJsonValue jv;
    if (data.isValid())
    {
        if (QString(data.typeName()) == "QByteArray")
        {
            // QByteArray to string
            QJsonObject jo;
            QByteArray ba = data.toByteArray();
            QString s = ba.toHex();
            jo["array"] = s;
            jv = jo;
        }
        else
            jv = QJsonValue::fromVariant(data);
    }
    else
        jv = QJsonValue::fromVariant(QVariant());
    return jv;
}

QVariant DapDataSettings::unpackItem(const QJsonValue& data)
{
    QVariant v;
    if (data.isObject())
    {
        QJsonObject jo = data.toObject();
        QString key = "array";
        if (jo.contains(key))
        {
            // string to QVarinat(QByteArray)
            QString sdata = jo.value(key).toString();
            QByteArray ba(sdata.toStdString().c_str(), sdata.length());
            v = QVariant(QByteArray::fromHex(ba.toStdString().c_str()));
        }
    }
    else if (data.isNull() || data.isUndefined())
        v = QVariant();
    else
        v = data.toVariant();
    return v;
}

bool DapDataSettings::itemCompare(const QVariant& a, const QVariant& b)
{
    if (QString(a.typeName()) == QString(b.typeName()))
    {
        if (QString(a.typeName()) == "QByteArray")
        {
            return a.toByteArray().toHex() == b.toByteArray().toHex();
        }
        else
            return a == b;
    }
    else
        return false;
}


/// DapDataSettingsLocal

QSettings* DapDataSettingsLocal::settings()
{
#ifdef Q_OS_ANDROID
    static QString s_path = DapLogger::defaultLogPath(DAP_BRAND).chopped(3).append("settings.ini");

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

    static QSettings s_settings(s_path, QSettings::IniFormat);
#else
    static QSettings s_settings;
#endif
    return &s_settings;
}

QVariant DapDataSettingsLocal::getSetting(const QString &a_key)
{
    return settings()->value(a_key);
}

void DapDataSettingsLocal::saveSetting(const QString &a_key, const QVariant &a_value)
{
    settings()->setValue(a_key, a_value);
    settings()->sync();
}

void DapDataSettingsLocal::removeSetting(const QString &a_key)
{
    settings()->remove(a_key);
}

QJsonObject DapDataSettingsLocal::toJson()
{
    QJsonObject settingData;
    foreach(const QString& key, settings()->allKeys())
        settingData[key] = DapDataSettings::packItem(getSetting(key));
    return settingData;
}



QVariant DapDataSettingsMap::getSetting(const QString &a_key)
{
    if (m_localData.contains(a_key))
        return m_localData[a_key];
    else
        return QVariant();
}

void DapDataSettingsMap::saveSetting(const QString &a_key, const QVariant &a_value)
{
    QMap<QString, QVariant> data;
    bool n_eaqual = !DapDataSettings::itemCompare(m_localData[a_key], a_value);
    // if m_localData[a_key] and a_value are different
    if (n_eaqual)
    {
        // then save the new data and emit a signal
        m_localData[a_key] = a_value;
        data[a_key] = a_value;
        emit dataUpdated(data);
    }
}

void DapDataSettingsMap::removeSetting(const QString &a_key)
{
    QStringList keys;
    m_localData.remove(a_key);
    keys.append(a_key);
    emit dataRemoved(keys);
}


QJsonObject DapDataSettingsMap::toJson()
{
    QJsonObject settingData;
    foreach(const QString& key, m_localData.keys())
        settingData[key] = DapDataSettings::packItem(getSetting(key));
    return settingData;
}


void DapDataSettingsMap::fromJson(QJsonObject* jdata)
{
    foreach(const QString& key, jdata->keys())
        if (jdata->value(key) != QJsonValue::Undefined)
        {
            m_localData[key] = DapDataSettings::unpackItem(jdata->value(key).toObject());
        }
}




DapDataLocal::DapDataLocal()
    : QObject()
    , secretKey(Q_NULLPTR)
    , m_loadAuthorizationDatas(false)
    , m_serialKeyData(new DapSerialKeyData(this))
    , m_buReportHistory(new DapBugReportHistory(this))
    , m_localSettings(new DapDataSettingsLocal())
    , m_serviceSettings(new DapDataSettingsMap())
{
    qDebug() << "[DL] DapDataLocal Constructor";
//    initSecretKey();
//    this->loadAuthorizationDatas();
}

/// DapDataLocal class initialization on the gui side
void DapDataLocal::initGuiData()
{
    // signal connection for send data to service
    connect(m_serviceSettings, &DapDataSettingsMap::dataUpdated,  this, &DapDataLocal::dataUpdated);
    // signal connection for send removed keys
    connect(m_serviceSettings, &DapDataSettingsMap::dataRemoved,  this, &DapDataLocal::dataRemoved);
    // list of settings keys located on the service
    m_keysForServerStorage << TEXT_SERIAL_KEY << TEXT_SERIAL_KEY_HISTORY << TEXT_PENDING_SERIAL_KEY
                           << TEXT_BUGREPORT_HISTORY << TEXT_LOGIN << TEXT_PASSWORD << TEXT_TX_OUT
                           << TEXT_KEY;
    qDebug() << "[DL] DapDataLocal: init gui data";
}

/// DapDataLocal class initialization on the service side
void DapDataLocal::initServiceData()
{
    // load configuration data from xml file
    config.parseXML(":/data.xml");
    qDebug() << "[DL] DapDataLocal: init service data";
}


/// Get login.
/// @return Login.
QString DapDataLocal::login() const
{
    return m_login;
}

/// Set login.
/// @param login Login.
void DapDataLocal::setLogin(const QString &a_login)
{
    if (this->m_login == a_login)
        return;
    m_login = a_login;

    emit loginChanged(m_login);
}

/// Get password.
/// @return Password.
QString DapDataLocal::password() const
{
    return m_password;
}

/// Set password.
/// @param password Password.
void DapDataLocal::setPassword(const QString &a_password)
{
    if (this->m_password == a_password)
        return;
    this->m_password = a_password;

    emit this->passwordChanged(m_password);
}

void DapDataLocal::saveAuthorizationData()
{
    this->saveEncryptedSetting(TEXT_LOGIN     , this->login());
    this->saveEncryptedSetting(TEXT_PASSWORD  , this->password());
}

void DapDataLocal::saveSerialKeyData()
{
    if (m_serialKeyData)
        this->saveToSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
}

void DapDataLocal::resetSerialKeyData()
{
    if (m_serialKeyData){
        m_serialKeyData->reset();
        this->saveToSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
    }
}

void DapDataLocal::savePendingSerialKey(QString a_serialkey)
{
    m_pendingSerialKey = a_serialkey;
    this->saveToSettings(TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
}

void DapDataLocal::saveHistoryData(QString a_type, QString a_data)
{
    if (a_data.isEmpty())
        return;
    QList<QString> m_tempHistoryDataList;
    this->loadFromSettings(a_type, m_tempHistoryDataList);
    if (!m_tempHistoryDataList.contains(a_data))
        m_tempHistoryDataList.prepend(a_data);
    this->saveToSettings(a_type, m_tempHistoryDataList);

    emit sigHistoryDataSaved();
}

void DapDataLocal::removeItemFromHistory(QString a_type, QString a_item){

  if (a_item.isEmpty())
    return;
  QList<QString> m_tempHistoryDataList;
  this->loadFromSettings(a_type, m_tempHistoryDataList);

  a_item.remove(QRegExp("[^0-9]"));
  QMutableListIterator<QString> it (m_tempHistoryDataList);
  while(it.hasNext()) {
    QString item = it.next();
    if (item == a_item){
      qDebug() << "remove " + item + " from " + a_type;
      it.remove();
    }
  }

  this->saveToSettings(a_type, m_tempHistoryDataList);

  emit sigHistoryDataSaved();
}

QList<QString> DapDataLocal::getHistorySerialKeyData()
{
    QList<QString> m_tempHistoryDataList;
    this->loadFromSettings(TEXT_SERIAL_KEY_HISTORY, m_tempHistoryDataList);
    return m_tempHistoryDataList;
}

void DapDataLocal::loadAuthorizationDatas()
{
#ifdef Q_OS_ANDROID
    auto keys = m_localSettings->allKeys();
    if (keys.isEmpty()) {
        QSettings oldSettings;
        for (auto key : oldSettings.allKeys()) {
            m_localSettings->saveSetting(key, oldSettings.value(key));
        }
    }
#endif

    this->setLogin(getEncryptedSetting(TEXT_LOGIN).toString());
    this->setPassword(getEncryptedSetting(TEXT_PASSWORD).toString());

    if (m_serialKeyData)
        this->loadFromSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
    this->loadFromSettings(TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
}

QVariant DapDataLocal::getEncryptedSetting(const QString &a_setting)
{
    QByteArray outString;
    this->loadEncryptedSettingString(a_setting, outString);
    return QString(outString);
}

bool DapDataLocal::loadEncryptedSettingString(const QString &a_setting, QByteArray& a_outString)
{
    QVariant varSettings = DapDataLocal::getSetting(a_setting);

    if (!varSettings.isValid() || !varSettings.canConvert<QByteArray>())
        return false;

    QByteArray encryptedString = varSettings.toByteArray();
    if (encryptedString.isEmpty())
    {
        a_outString = "";
        return true;
    }
    if (secretKey == Q_NULLPTR) initSecretKey();
    secretKey->decode(encryptedString, a_outString);

    return true;
}


void DapDataLocal::saveEncryptedSetting(const QString &a_setting, const QVariant &a_value)
{
    this->saveEncryptedSetting(a_setting, a_value.toByteArray());
}

void DapDataLocal::saveEncryptedSetting(const QString &a_setting, const QByteArray &a_string)
{
    QByteArray encodedString;
    if (secretKey == Q_NULLPTR) initSecretKey();
    secretKey->encode(a_string, encodedString);
    DapDataLocal::saveSetting(a_setting, encodedString);
}

DapDataSettings* DapDataLocal::settings(const QString& keyName)
{
    if (m_keysForServerStorage.contains(keyName))
        return m_serviceSettings;
    else
        return m_localSettings;
}

QVariant DapDataLocal::getSetting(const QString &a_setting)
{
    DapDataSettings* settings = DapDataLocal::instance()->settings(a_setting);
    return settings->getSetting(a_setting);
}

void DapDataLocal::saveSetting(const QString &a_setting, const QVariant &a_value)
{
    DapDataSettings* settings = DapDataLocal::instance()->settings(a_setting);
    settings->saveSetting(a_setting, a_value);
}

void DapDataLocal::removeSetting(const QString &a_setting)
{
    DapDataSettings* settings = DapDataLocal::instance()->settings(a_setting);
    settings->removeSetting(a_setting);
}

DapBugReportData *DapDataLocal::bugReportData()
{
    return DapBugReportData::instance();
}

DapServersData *DapDataLocal::serversData()
{
    return DapServersData::instance();
}

DapSerialKeyData *DapDataLocal::serialKeyData()
{
    if (!m_loadAuthorizationDatas)
    {
        loadAuthorizationDatas();
        m_loadAuthorizationDatas = true;
    }
    return m_serialKeyData;
}

DapBugReportHistory * DapDataLocal::bugReportHistory()
{
    return m_buReportHistory;
}

void DapDataLocal::initSecretKey()
{
    if (m_localSettings->getSetting("key").toString().isEmpty())
    {
        m_localSettings->saveSetting("key", getRandomString(40));
    }
    if (secretKey != nullptr) {
        delete secretKey;
    }
    secretKey = new DapKey(DAP_ENC_KEY_TYPE_IAES, m_localSettings->getSetting("key").toString() + "SLKJGN234njg6vlkkNS3s5dfzkK5O54jhug3KUifw23");
}

QString DapDataLocal::getRandomString(int size)
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

void DapDataLocal::updateSettingWithServiceSecretKey()
{
    QLocale::Language systemLanguage;
    QLocale::Language savedLanguage;
    QString ColorTheme = m_localSettings->getSetting(SETTING_THEME).toString();
    loadFromSettings(SETTING_SYS_LOCALE, systemLanguage);
    loadFromSettings(SETTING_LOCALE, savedLanguage);
    m_localSettings->saveSetting("key", m_serviceSettings->getSetting("key").toString());
    initSecretKey();
    m_localSettings->saveSetting(SETTING_THEME, ColorTheme);
    saveToSettings(SETTING_SYS_LOCALE, systemLanguage);
    saveToSettings(SETTING_LOCALE, savedLanguage);
}

bool DapDataLocal::compareLocalAndServiceSecretKeys()
{
    return m_localSettings->getSetting("key").toString() == m_serviceSettings->getSetting("key").toString();
}

bool DapDataLocal::isServiceSecretKeyEmpty()
{
    return m_serviceSettings->getSetting("key").toString().isEmpty() || m_serviceSettings->getSetting("key").toString().isNull();
}

void DapDataLocal::saveLocalSecretKeyToService()
{
    m_serviceSettings->saveSetting("key", m_localSettings->getSetting("key"));
}

DapDataLocal *DapDataLocal::instance()
{
    static DapDataLocal s_instance;
    return &s_instance;
}
