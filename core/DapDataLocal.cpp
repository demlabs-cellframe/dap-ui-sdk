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

DapDataLocal::picturesMap DapDataLocal::m_pictruePath = {
    {DapServerLocation::ENGLAND, ":/country/GB.png"},
    {DapServerLocation::FRANCE, ":/country/FR.png"},
    {DapServerLocation::GERMANY, ":/country/DE.png"},
    {DapServerLocation::USA, ":/country/US.png"},
    {DapServerLocation::NETHERLANDS, ":/country/NL.png"},
    {DapServerLocation::RUSSIA, ":/country/RU.png"},
    {DapServerLocation::UKRAINE, ":/country/UA.png"},
    {DapServerLocation::UNKNOWN, ":/country/Flag_gf.png"},
    {DapServerLocation::Netherlands, ":/country/NL.png"},
    {DapServerLocation::Singapore, ":/country/SG.png"},
    {DapServerLocation::Germany, ":/country/DE.png"},
};

DapDataLocal::DapDataLocal()
    : QObject()
    , m_serialKeyData(new DapSerialKeyData(this))
{
    qDebug() << "[DL] DapDataLocal Constructor";

    parseXML(":/data.xml");
    
    this->loadAuthorizationDatas();
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
                                    item.location = DapServerInfo::stringToLocation(sr->readElementText());
                                } else {
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
#ifdef  QT_DEBUG
    DapDataLocal::serversData()->addServer(DapServerLocation::UNKNOWN, "local", "127.0.0.1",  8099);
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
    this->saveEncriptedSetting(this->TEXT_LOGIN     , this->login());
    this->saveEncriptedSetting(this->TEXT_PASSWORD  , this->password());
}

void DapDataLocal::saveSerialKeyData()
{
    if (m_serialKeyData)
        this->saveToSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
}

void DapDataLocal::loadAuthorizationDatas()
{
    this->setLogin(getEncriptedSetting(TEXT_LOGIN).toString());
    this->setPassword(getEncriptedSetting(TEXT_PASSWORD).toString());

    if (m_serialKeyData)
        this->loadFromSettings(TEXT_SERIAL_KEY, *m_serialKeyData);
}

void DapDataLocal::rotateCDBList() {
    if (m_cdbServersList.size() > 1) {
        auto tmp = m_cdbServersList.takeFirst();
        m_cdbServersList.push_back(tmp);
    }
}

QSettings* DapDataLocal::settings()
{
    static QSettings s_settings;
    return &s_settings;
}

QVariant DapDataLocal::getEncriptedSetting(const QString &a_setting)
{
    QByteArray outString;
    this->loadEncriptedSettingString(a_setting, outString);
    return QString(outString);
}

bool DapDataLocal::loadEncriptedSettingString(const QString &a_setting, QByteArray& a_outString)
{
    QVariant varSettings = DapDataLocal::getSetting(a_setting);

    if (!varSettings.isValid() || !varSettings.canConvert<QByteArray>())
        return false;

    QByteArray encriptedString = varSettings.toByteArray();
    if (encriptedString.isEmpty())
    {
        a_outString = "";
        return true;
    }

    this->initSecretKey();
    secretKey->decode(encriptedString, a_outString);

    return true;
}


void DapDataLocal::saveEncriptedSetting(const QString &a_setting, const QVariant &a_value)
{
    this->saveEncriptedSetting(a_setting, a_value.toByteArray());
}

void DapDataLocal::saveEncriptedSetting(const QString &a_setting, const QByteArray &a_string)
{
    initSecretKey();
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

bool DapDataLocal::initSecretKey()
{
    if (settings()->value("key").toString().isEmpty())
    {
        settings()->setValue("key", getRandomString(40));
    }
    if (secretKey != nullptr) {
        delete secretKey;
    }
    secretKey = new DapKeyAes();
    QString kexString = settings()->value("key").toString() + "SLKJGN234njg6vlkkNS3s5dfzkK5O54jhug3KUifw23";
    return secretKey->init(QString(kexString));
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

/**
 * @brief DataLocal::locationToIcon
 * @param a_location
 * @return
 */
QString DapDataLocal::locationToIconPath(DapServerLocation loc)
{
    QString locPath = m_pictruePath.value(loc);
    if (locPath == "") {
        qWarning() << "Not found picture for current location. Return default!";
        return m_pictruePath.value(DapServerLocation::UNKNOWN);
    }
    return locPath;
}

DapDataLocal *DapDataLocal::instance()
{
    static DapDataLocal s_instance;
    return &s_instance;
}

void DapDataLocal::setUrlUpdate(const QString &a_url)
{
    m_urlUpdate = a_url;
}

QString DapDataLocal::urlUpdate() const
{
    return m_urlUpdate;
}
