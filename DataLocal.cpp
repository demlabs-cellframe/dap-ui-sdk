#include <QStandardPaths>

#include <QStandardItemModel>
#include <QXmlStreamReader>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QSettings>
#include <QCoreApplication>
#include <algorithm>

#include "DataLocal.h"

DataLocal::picturesMap DataLocal::m_pictruePath = {
    {DapServerLocation::ENGLAND, ":/country/Flag_uk.png"},
    {DapServerLocation::FRANCE, ":/country/Flag_fr.png"},
    {DapServerLocation::GERMANY, ":/country/Flag_de.png"},
    {DapServerLocation::USA, ":/country/Flag_us.png"},
    {DapServerLocation::NETHERLANDS, ":/country/Flag_nl.png"},
    {DapServerLocation::RUSSIA, ":/country/Flag_ru.png"},
    {DapServerLocation::UKRAINE, ":/country/Flag_ua.png"},
    {DapServerLocation::UNKNOWN, ":/country/ussr.png"},
};

DataLocal *DataLocal::_me = Q_NULLPTR;

DataLocal::DataLocal()
{
    qDebug() << "[DL] Constructor";
    parseXML(":/data.xml");
    setServerName(serverTheBest().name);
}

void DataLocal::parseXML(const QString& a_fname)
{
    QFile file(a_fname);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "[DL] Can't open data.xml from built in resource";
        return;
    }

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
                                    item.location = DapServerInfo::stringToLaction(sr->readElementText());
                                } else {
                                    qDebug() << "[DL] Inside tag 'server': Unknown tag "<<sr->name();
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
                }else{
                    qDebug() << "[DL] Inside tag 'data' unknown tag "<<sr->name();
                    //sr->skipCurrentElement();
                }
            }
        }
    }
#ifdef  QT_DEBUG
    addServer(DapServerLocation::FRANCE, "fr-dev", "163.172.29.34", 8002);
    addServer(DapServerLocation::NETHERLANDS, "nl-dev", "ap-nl-0.divevpn.demlabs.net", 8002);
    addServer(DapServerLocation::NETHERLANDS, "local", "127.0.0.1",  8002);
    addServer(DapServerLocation::NETHERLANDS, "VB", "192.168.1.104",  8002);
#endif


    delete sr;
}

void DataLocal::addServer(const DapServerInfo& dsi) {
    for(auto& s: servers()) {
        if(s == dsi) {
            qWarning() << "[DataLocal::addServer] Server "
                          "already exists in list";
            return;
        }
    }
    m_servers.push_back(dsi);
}

void DataLocal::setServerTheBest(const DapServerInfo &server){
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
QString DataLocal::login() const
{
    return mLogin;
}

/// Set login.
/// @param login Login.
void DataLocal::setLogin(const QString &login)
{
    mLogin = login;
    emit loginChanged(mLogin);
}

/// Get password.
/// @return Password.
QString DataLocal::password() const
{
    return mPassword;
}

/// Set password.
/// @param password Password.
void DataLocal::setPassword(const QString &password)
{
    mPassword = password;
    emit passwordChanged(mPassword);
}

/// Get server name.
/// @return Server name.
QString DataLocal::serverName() const
{
    return mServerName;
}

/// Set server name.
/// @param server Server name.
void DataLocal::setServerName(const QString &serverName)
{
    if (serverName != mServerName) {
        mServerName = serverName;
        emit serverNameChanged(mServerName);
    }
}

/// Get server name by its address.
/// @param address Server address.
/// @return Server name.
QString DataLocal::getServerNameByAddress(const QString &address)
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
DataLocal::connectComboBox(QComboBox *a_comboBox)
{
    //synchronize:
    a_comboBox->setCurrentText(serverName());

    //connection when serverName in DataLocale changed
    connect(this,       SIGNAL(serverNameChanged(QString)),
            a_comboBox, SLOT  (setCurrentText(QString)));

    //connection when comboBoxChanged
    connect(a_comboBox, SIGNAL(currentTextChanged(QString)),
            this,       SLOT  (setServerName(QString)));
}

void DataLocal::addServer(DapServerLocation location, const QString& name,
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
QString DataLocal::locationToIconPath(DapServerLocation loc)
{
    QString locPath = m_pictruePath.value(loc);
    if (locPath == "") {
        qWarning() << "Not found picture for current location. Return default!";
        return m_pictruePath.value(DapServerLocation::UNKNOWN);
    }
    return locPath;
}
