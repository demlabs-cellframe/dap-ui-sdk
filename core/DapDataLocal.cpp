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

#include "dap_net.h"

#ifdef DAP_OS_ANDROID
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

/* VARIABLES */

static const QMap<Authorization, QString> s_authTypeMap =
{
  { Authorization::account,     "account" },
  { Authorization::serialKey,   "serialKey" },
  { Authorization::certificate, "certificate" },
  { Authorization::undefined,   "" },
};

static DapDataLocal *s_instance = nullptr;


DapDataLocal::DapDataLocal()
  : QObject()
  , m_serialKeyData(new DapSerialKeyData (this))
  , m_bugReportHistory(new DapBugReportHistory (this))
  , m_serialKeyHistory (new DapSerialKeyHistory (this))
{
  s_instance = this;


}

DapDataLocal::~DapDataLocal()
{

}

void DapDataLocal::_parseXML(const QString& a_fname)
{
    QFile file(a_fname);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Can't open data.xml from built in resource";
        return;
    }
    qDebug() << "data.xml opened, size "<< file.size();
    QXmlStreamReader *sr = new QXmlStreamReader(&file);
    if(sr->readNextStartElement())
    {
        if(sr->name().toString() == "data")
        {
            while ( sr->readNextStartElement() )
            {
                if( sr->name().toString() == "servers")
                {
                    while ( sr->readNextStartElement() )
                    {
                        if( sr->name().toString() == "server") 
                        {
                            DapServerInfo item;
                            while ( sr->readNextStartElement() )
                            {
                                if(sr->name().toString() == "name")
                                {
                                    item.setName (sr->readElementText());
                                }
                                else if (sr->name().toString() == "address") 
                                {
                                    item.setAddress (sr->readElementText());
                                }
                                else if( sr->name().toString() == "port") 
                                {
                                    bool ok;
                                    quint16 port = quint16(sr->readElementText().toInt(&ok));
                                    if (!ok) 
                                    {
                                        throw std::runtime_error("Can't cast port to int "
                                                                 "from XML file");
                                    }
                                    item.setPort (port);
                                }
                                else if(sr->name().toString() == "location") 
                                {
                                    item.setLocation (sr->readElementText());
                                }
                                else if (sr->name().toString() == "state") 
                                {
                                    item.setOnline (sr->readElementText());
                                }
                                else 
                                {
                                    qWarning() << "[DL] Inside tag 'server': Unknown tag "<<sr->name();
                                    sr->skipCurrentElement();
                                }
                            }
                            qDebug() << "[DL] Server "<<item.name()<<" added";
                            DapServerList::instance()->append (std::move (item));
                        }
                        else
                        {
                            qDebug() << "[DL] Inside tag 'servers': unknown tag "<<sr->name();
                            sr->skipCurrentElement();
                        }
                    }
                }
                else if( sr->name().toString() == "cdb")
                {
                    struct sockaddr_storage l_addr_out = {};
                    QByteArray l_cdb_addr_qstr = sr->readElementText().toLatin1();
                    char *l_cdb_addr = l_cdb_addr_qstr.data();
                    char l_host[DAP_HOSTADDR_STRLEN + 1] = { '\0' }; uint16_t l_port = 0;
                    if(dap_net_parse_config_address(l_cdb_addr, l_host, &l_port, NULL, NULL)
                                && dap_net_resolve_host(l_host, dap_itoa(l_port), false, &l_addr_out, NULL))
                    {
                        DapCdbServer l_cdbServerAddr;
                        char l_addr_out_str[NI_MAXHOST] = {0};
                        char servInfo[NI_MAXSERV];
                        if (!getnameinfo((struct sockaddr*)&l_addr_out, sizeof(l_addr_out), l_addr_out_str, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV))
                        {
                            l_cdbServerAddr.address = QString(l_addr_out_str);
                            l_cdbServerAddr.port = l_port ? l_port : 80;
                            m_cdbServersList.push_back (l_cdbServerAddr);
                            qInfo() << "Add CDB address: " << m_cdbServersList.back().address;
                        }
                    }
                }
                else if( sr->name().toString() == "network-default")
                {
                    m_networkDefault = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }
                else if( sr->name().toString() == "kelvpn-pub")
                {
                    struct sockaddr_storage l_addr_out = {};
                    QByteArray l_pub_addr_qstr = sr->readElementText().toLatin1();
                    char *l_pub_addr = l_pub_addr_qstr.data();
                    char l_host[DAP_HOSTADDR_STRLEN + 1] = { '\0' }; uint16_t l_port = 0;
                    if(dap_net_parse_config_address(l_pub_addr, l_host, &l_port, NULL, NULL)
                                && dap_net_resolve_host(l_host, dap_itoa(l_port), false, &l_addr_out, NULL)){
                        char l_addr_out_str[NI_MAXHOST] = {0};
                        char servInfo[NI_MAXSERV];
                        if (!getnameinfo((struct sockaddr*)&l_addr_out, sizeof(l_addr_out), l_addr_out_str, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV)){
                            m_kelvpnPub = QString(l_addr_out_str);
                            qInfo() << "KelVPN pub address: " << m_kelvpnPub;
                        }
                    }
                }
                else if( sr->name().toString() == "min-node-version")
                {
                    m_minNodeVersion = sr->readElementText();
                    qInfo() << "Min node version: " << m_minNodeVersion;
                }
                else if( sr->name().toString() == "min-dashboard-version")
                {
                    m_minDashboardVersion = sr->readElementText();
                    qInfo() << "Min dashboard version: " << m_minDashboardVersion;
                }
                else if( sr->name().toString() == "title")
                {
                    m_brandName = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }
                else if( sr->name().toString() == "url_site")
                {
                    m_urlSite = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }
                else
                {
                    qDebug() << "[DL] Inside tag 'data' unknown tag "<<sr->name();
                    sr->skipCurrentElement();
                }
            }
        }
    }
    file.close();
#ifdef  QT_DEBUG
    DapServerList::instance()->append (DapServerInfo {"UNKNOWN", "local", "127.0.0.1",  8099});
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

    emit sigLoginChanged(m_login);
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

    emit this->sigPasswordChanged(m_password);
}

void DapDataLocal::saveAuthorizationData()
{
  saveEncryptedSetting (TEXT_LOGIN, login());
  saveEncryptedSetting (TEXT_PASSWORD, password());
}

void DapDataLocal::setLogFilePath (const QString &a_path)
{
  m_logFilePath = a_path;
}

const QString &DapDataLocal::getLogFilePath()
{
  return m_logFilePath;
}

void DapDataLocal::setLogPath (const QString &a_path)
{
  m_logPath = a_path;
}

const QString &DapDataLocal::getLogPath()
{
  return m_logPath;
}

const DapCdbServerList &DapDataLocal::cdbServersList() const
{
  return m_cdbServersList;
}

const QString &DapDataLocal::KelvpnPub() const
{
  return m_kelvpnPub;
}

const QString &DapDataLocal::networkDefault() const
{
  return m_networkDefault;
}

const QString &DapDataLocal::getUrlSite() const
{
  return m_urlSite;
}

const QString &DapDataLocal::getBrandName() const
{
  return m_brandName;
}

const QString &DapDataLocal::getMinDashboardVersion() const
{
  return m_minDashboardVersion;
}

const QString &DapDataLocal::getMinNodeVersion() const
{
  return m_minNodeVersion;
}

const QString &DapDataLocal::getCountryISO() const
{
  return m_coutryISO;
}

void DapDataLocal::setCountryISO(const QString &a_iso_code)
{
  m_coutryISO = a_iso_code;
}

void DapDataLocal::saveSerialKeyData()
{
  if (m_serialKeyData)
    this->saveToSettings (TEXT_SERIAL_KEY, *m_serialKeyData);
}

void DapDataLocal::resetSerialKeyData()
{
  if (m_serialKeyData)
  {
    m_serialKeyData->reset();
    this->saveToSettings (TEXT_SERIAL_KEY, *m_serialKeyData);
  }
}

void DapDataLocal::savePendingSerialKey(const QString &a_serialkey)
{
  m_pendingSerialKey = a_serialkey;
  this->saveToSettings (TEXT_PENDING_SERIAL_KEY, m_pendingSerialKey);
}

const QString &DapDataLocal::pendingSerialKey() const
{
  return m_pendingSerialKey;
}

void DapDataLocal::_loadAuthorizationDatas()
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

QVariant DapDataLocal::getEncryptedSetting (const QString &a_setting) const
{
  QByteArray outString;
  this->loadEncryptedSettingString (a_setting, outString);
  return QString (outString);
}

bool DapDataLocal::loadEncryptedSettingString (const QString &a_setting, QByteArray &a_outString) const
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


void DapDataLocal::saveEncryptedSetting (const QString &a_setting, const QVariant &a_value)
{
    this->saveEncryptedSetting (a_setting, a_value.toByteArray());
}

void DapDataLocal::saveEncryptedSetting (const QString &a_setting, const QByteArray &a_string)
{
    QByteArray encodedString;
    secretKey->encode (a_string, encodedString);
    DapDataLocal::saveSetting (a_setting, encodedString);
}

QVariant DapDataLocal::getSetting (const QString &a_setting)
{
  return instance()->settings()->value (a_setting);
}

void DapDataLocal::saveSetting (const QString &a_setting, const QVariant &a_value)
{
  auto stg = instance()->settings();
  stg->setValue (a_setting, a_value);
  stg->sync();
}

void DapDataLocal::removeSetting (const QString &a_setting)
{
  instance()->settings()->remove (a_setting);
}

DapBugReportData *DapDataLocal::bugReportData()
{
    return DapBugReportData::instance();
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

void DapDataLocal::_initSecretKey()
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

QString DapDataLocal::getRandomString (int a_size)
{
  /* variables */
  static const QString possibleCharacters ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  QString randomString;

  for (int i=0; i < a_size; ++i)
  {
    int index       = qrand() % possibleCharacters.length();
    QChar nextChar  = possibleCharacters.at (index);
    randomString.append (nextChar);
  }

  return randomString;
}

Authorization DapDataLocal::authorizationType() const
{
  auto auth = getSetting (SETTING_AUTHORIZATION).toString();

  Authorization defaultAuth =
#ifdef BRAND_RISEVPN
    Authorization::undefined;
#else // BRAND_RISEVPN
    Authorization::account;
#endif // BRAND_RISEVPN

  return s_authTypeMap.key (auth, defaultAuth);
}

void DapDataLocal::setAuthorizationType (Authorization a_type)
{
  saveSetting (SETTING_AUTHORIZATION, s_authTypeMap.value (a_type, QString()));
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
  return s_instance;
}

/********************************************
 * DapCdbServer
 *******************************************/

QString DapCdbServer::toString() const
{
  return QString ("%1:%2").arg (address).arg (port);
}

void DapCdbServer::fromString (const QString &a_src)
{
  auto source = a_src.split (':');
  address     = source.constFirst();
  port        = source.constLast().toInt();
  if (port == 0)
    port = 80;
}

DapCdbServer DapCdbServer::serverFromString (const QString &a_src)
{
  DapCdbServer result;
  result.fromString (a_src);
  return result;
}

/********************************************
 * DapCdbServerList
 *******************************************/

DapCdbServerList DapCdbServerList::toServers (const QStringList &a_src)
{
  DapCdbServerList result;
  for (const auto &item : a_src)
    result << DapCdbServer::serverFromString (item);
  return result;
}

QStringList DapCdbServerList::toStrings (const DapCdbServerList &a_servers)
{
  QStringList result;
  for (const auto &item : a_servers)
    result << item.toString();
  return result;
}

/*-----------------------------------------*/
