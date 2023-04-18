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
#include "DapSerialKeyHistory.h"
#include "DapLogger.h"

#ifdef DAP_OS_ANDROID
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

DapDataLocal::DapDataLocal()
  : QObject()
  , m_serialKeyData(new DapSerialKeyData (this))
  , m_bugReportHistory(new DapBugReportHistory (this))
  , m_serialKeyHistory (new DapSerialKeyHistory (this))
  , m_packageList(new QSet<QString>)
{
    qDebug() << "[DL] DapDataLocal Constructor";
    parseXML(":/data.xml");
    initSecretKey();
    this->loadAuthorizationDatas();
    _syncCdbWithSettings();
}

void DapDataLocal::parseXML(const QString& a_fname)
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
                    m_cdbServersList.push_back (DapCdbServer::serverFromString (sr->readElementText()));
                    qInfo() << "Add CDB address: " << m_cdbServersList.back().address;
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

//void DapDataLocal::saveHistoryData(QString a_type, QString a_data)
//{
//    if (a_data.isEmpty())
//        return;
//    QList<QString> m_tempHistoryDataList;
//    this->loadFromSettings(a_type, m_tempHistoryDataList);
//    if (!m_tempHistoryDataList.contains(a_data))
//        m_tempHistoryDataList.prepend(a_data);
//    this->saveToSettings(a_type, m_tempHistoryDataList);

//    emit sigHistoryDataSaved(a_type);
//}

//void DapDataLocal::removeItemFromHistory(QString a_type, QString a_item){

//  if (a_item.isEmpty())
//    return;
//  QList<QString> m_tempHistoryDataList;
//  this->loadFromSettings(a_type, m_tempHistoryDataList);

//  a_item.remove(QRegExp("[^0-9]"));
//  QMutableListIterator<QString> it (m_tempHistoryDataList);
//  while(it.hasNext()) {
//    QString item = it.next();
//    if (item == a_item){
//      qDebug() << "remove " + item + " from " + a_type;
//      it.remove();
//    }
//  }

//  this->saveToSettings(a_type, m_tempHistoryDataList);

//  emit sigHistoryDataSaved(a_type);
//}

//QList<QString> DapDataLocal::getHistorySerialKeyData()
//{
//    QList<QString> m_tempHistoryDataList;
//    this->loadFromSettings(TEXT_SERIAL_KEY_HISTORY, m_tempHistoryDataList);
//    return m_tempHistoryDataList;
//}

void DapDataLocal::loadAuthorizationDatas()
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
        this->loadFromSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
    this->loadFromSettings(TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
}

void DapDataLocal::_syncCdbWithSettings()
{
  /* vars */
  static const QString SETTING_CDB { "cdb" };

  /* get from settings */
  auto cdbCfg = getSetting (SETTING_CDB);

  /* if not found, store cdb's from built-in list (if not empty) */
  if (!cdbCfg.isValid())
    return;

  /* if found, replace built-in cdb's with list provided from settings */
  auto list   = QString::fromLatin1 (QByteArray::fromBase64 (cdbCfg.toByteArray())).split(',');
  auto result = DapCdbServerList::toServers (list);
  updateCdbList (result);
}

QSettings* DapDataLocal::settings()
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
    secretKey->encode(a_string, encodedString);
    DapDataLocal::saveSetting(a_setting, encodedString);
}

QVariant DapDataLocal::getSetting(const QString &a_setting)
{
    return settings()->value(a_setting);
}

void DapDataLocal::saveSetting(const QString &a_setting, const QVariant &a_value)
{
    settings()->setValue(a_setting, a_value);
    settings()->sync();
}

void DapDataLocal::removeSetting(const QString &a_setting)
{
    settings()->remove(a_setting);
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
    return m_serialKeyData;
}

DapBugReportHistory * DapDataLocal::bugReportHistory()
{
  return m_bugReportHistory;
}

DapSerialKeyHistory *DapDataLocal::serialKeyHistory()
{
  return m_serialKeyHistory;
}

QSet<QString> *DapDataLocal::packageList()
{
  return m_packageList;
}

QMap<QString, QMap<QString, QImage>> *DapDataLocal::appList()
{
  return m_appList;
}


void DapDataLocal::initSecretKey()
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

Authorization DapDataLocal::authorizationType()
{
    auto auth = DapDataLocal::instance()->getSetting (SETTING_AUTHORIZATION).toString();
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

void DapDataLocal::setAuthorizationType(Authorization type)
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
    DapDataLocal::instance()->saveSetting (SETTING_AUTHORIZATION, auth);
}

void DapDataLocal::updateCdbList (const DapCdbServerList &a_newCdbList)
{
  /* clear old and store new */
  m_cdbServersList.clear();

  for (const auto &cdb : qAsConst(a_newCdbList))
    m_cdbServersList << cdb;

  /* update iterator */
  m_cdbIter = m_cdbServersList.constBegin();
}

DapDataLocal *DapDataLocal::instance()
{
    static DapDataLocal s_instance;
    return &s_instance;
}



QString DapCdbServer::toString() const
{
  return QString ("%1:%2").arg (address).arg (port);
}

void DapCdbServer::fromString(const QString &a_src)
{
  auto source = a_src.split (':');
  address     = source.constFirst();
  port        = source.constLast().toInt();
  if (port == 0)
    port = 80;
}

DapCdbServer DapCdbServer::serverFromString(const QString &a_src)
{
  DapCdbServer result;
  result.fromString (a_src);
  return result;
}

DapCdbServerList DapCdbServerList::toServers(const QStringList &a_src)
{
  DapCdbServerList result;
  for (const auto &item : a_src)
    result << DapCdbServer::serverFromString (item);
  return result;
}

QStringList DapCdbServerList::toStrings(const DapCdbServerList &a_servers)
{
  QStringList result;
  for (const auto &item : a_servers)
    result << item.toString();
  return result;
}
