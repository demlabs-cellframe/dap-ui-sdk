#include <QStandardPaths>

#include <QXmlStreamReader>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QSettings>
#include <QCoreApplication>
#include <algorithm>

#include "DapDataLocal.h"

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

DapDataLocal *DapDataLocal::_me = Q_NULLPTR;

DapDataLocal::DapDataLocal()
{
    qDebug() << "[DL] DapDataLocal Constructor";
    parseXML(":/data.xml");
    setServerName(m_servers.length() > 0 ? serverTheBest().name : "");
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
                            m_servers.push_back(item);
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
                }else if( sr->name() == "sign_up"){
                    urlSignUp = sr->readElementText();
                    qInfo() << "url for sign up: " << urlSignUp;
                }else{
                    qDebug() << "[DL] Inside tag 'data' unknown tag "<<sr->name();
                    sr->skipCurrentElement();
                }
            }
        }
    }
#ifdef  QT_DEBUG
    addServer(DapServerLocation::UNKNOWN, "local", "127.0.0.1",  8002);
#endif


    delete sr;
}

void DapDataLocal::addServer(const DapServerInfo& dsi) {

    m_servers.push_back(dsi);
}

/**
 * @brief DapDataLocal::clearServerList
 */
void DapDataLocal::clearServerList()
{
    m_servers.clear();
}



void DapDataLocal::setServerTheBest(const DapServerInfo &server){
    int index = m_servers.indexOf(server);
    if (index == -1) {
        qWarning() << "Server not found";
        return;
    } else {
        m_servers.move(index, 0);
    }
}

/// Get login.
/// @return Login.
QString DapDataLocal::login() const
{
    return mLogin;
}

/// Set login.
/// @param login Login.
void DapDataLocal::setLogin(const QString &login)
{
    mLogin = login;
    emit loginChanged(mLogin);
}

/// Get password.
/// @return Password.
QString DapDataLocal::password() const
{
    return mPassword;
}

/// Set password.
/// @param password Password.
void DapDataLocal::setPassword(const QString &password)
{
    mPassword = password;
    emit passwordChanged(mPassword);
}

/// Get server name.
/// @return Server name.
QString DapDataLocal::serverName() const
{
    return mServerName;
}

/// Set server name.
/// @param server Server name.
void DapDataLocal::setServerName(const QString &serverName)
{
    if (serverName != mServerName) {
        mServerName = serverName;
        emit serverNameChanged(mServerName);
    }
}

void DapDataLocal::saveSecretString(QString key, QString string){

    QSettings settings;
    initSecretKey();
    QByteArray tempStringIn = string.toUtf8(), tempStringOut;
    dapKey->encode(tempStringIn, tempStringOut);
    settings.setValue(key, tempStringOut);
}

QString DapDataLocal::getSecretString(QString key){
    QSettings settings;
    initSecretKey();
    QByteArray stringIn = settings.value(key).toByteArray(), stringOut;
    if (stringIn.isEmpty())
        return "";
    dapKey->decode(stringIn, stringOut);
    return QString(stringOut);
}

bool DapDataLocal::initSecretKey(){

    QSettings settings;
    if (settings.value("key").toString().isEmpty()){
        settings.setValue("key", getRandomString(40));
    }

    dapKey = new DapKeyAes();
    QString kexString = settings.value("key").toString() + "SLKJGN234njg6vlkkNS3s5dfzkK5O54jhug3KUifw23";
    dapKey->init(QString(kexString));
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

/// Get server name by its address.
/// @param address Server address.
/// @return Server name.
QString DapDataLocal::getServerNameByAddress(const QString &address)
{
    QList<DapServerInfo>::const_iterator it = std::find_if(m_servers.cbegin(), m_servers.cend(), 
        [=] (const DapServerInfo& server) 
        { 
            return server.address == address; 
        });
    
    if(it != m_servers.cend())
        return (*it).name;
    
    qCritical() << "Server with address " << address << " not found";
    
    return QString();
}

void /// Connect ComboBox to DataLocale. ComboBox currentText and DataLocale serverName will be synchronized
/// @param a_comboBox ComboBox
DapDataLocal::connectComboBox(QObject*a_comboBox)
{
    //connection when serverName in DataLocale changed
    connect(this,       SIGNAL(serverNameChanged(QString)),
            a_comboBox, SLOT  (setCurrentText(QString)));

    //connection when comboBoxChanged
    connect(a_comboBox, SIGNAL(currentTextChanged(QString)),
            this,       SLOT  (setServerName(QString)));
}

void DapDataLocal::addServer(DapServerLocation location, const QString& name,
                          const QString & address, quint16 port)
{
    DapServerInfo ss;
    ss.name = name;
    ss.location = location;
    ss.address = address;
    ss.port = port;
    addServer(std::move(ss));
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
