#include "DapServersLocalStorage.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>

DapServersLocalStorage::DapServersLocalStorage(QObject *parent,
                                               const QString& fileName)
    : QObject(parent)
{
    QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if(dataLocation.isEmpty()) {
        qCritical() << "Writable location cannot be determined";
        return;
    }

    if(!QDir(dataLocation).exists()) {
        if(!QDir().mkdir(dataLocation)) {
            qCritical() << "Can't create DataLocation dir";
            return;
        }
    }

    _serversListFilePath = QDir(dataLocation).filePath(fileName);
    qDebug() << "Servers list file path:" << _serversListFilePath;

    if( QFileInfo::exists(_serversListFilePath) )
        _loadServers();
}

int DapServersLocalStorage::_rewriteJsonFile()
{
    QJsonDocument d(_jsonServersListArr);

    QFile jsonFile(_serversListFilePath);
    if(!jsonFile.open(QFile::WriteOnly | QFile::Truncate)) {
        qWarning() << "Can't open servers list file"
                   << "filepath:" << _serversListFilePath;
        return 1;
    }
    if(jsonFile.write(d.toJson()) == -1) {
        qCritical() << "Error write servers to file";
        return 2;
    }
    return 0;
}

int DapServersLocalStorage::removeServer(const QString& address, const quint16 port)
{
    DapServerInfo s;
    s.setAddress (address);
    s.setPort (port);
    int rc = removeServer(s);
    if(rc == 0) emit changed();
    return rc;
}

int DapServersLocalStorage::editServer(const DapServerInfo& oldServer, const DapServerInfo& newServer)
{
    int rc = removeServer(oldServer);
    if(rc == 11) {
        qCritical() << "Server for edit not found";
        return 31;
    } else if(rc != 0) {
        return rc;
    }

    rc = addServer(newServer);
    if(rc == 0) emit changed();
    return rc;
}

int DapServersLocalStorage::removeServer(const DapServerInfo& server)
{
    int idx;
    if((idx = _serversList.indexOf(server)) == -1) {
        qWarning() << "Server not found";
        return 11;
    }
    _serversList.removeAt(idx);

    for(int i = 0; i < _jsonServersListArr.count(); i++) {
        auto obj = _jsonServersListArr[i].toObject();
        DapServerInfo tmp;
        DapServerInfo::fromJSON(obj, tmp);
        if(tmp == server) {
            qDebug() << "Removed from json object";
            _jsonServersListArr.removeAt(i);
        }
    }

    QJsonDocument d(_jsonServersListArr);
    QFile jsonFile(_serversListFilePath);
    if(!jsonFile.open(QFile::WriteOnly | QFile::Truncate)) {
        qWarning() << "Can't open servers list file"
                   << "filepath:" << _serversListFilePath;
        return 12;
    }
    if(jsonFile.write(d.toJson()) == -1) {
        qCritical() << "Error write servers to file";
        return 13;
    }

    emit changed();
    return _rewriteJsonFile();
}

bool DapServersLocalStorage::isServerExists(const DapServerInfo& server)
{
    for(auto&& srv: _serversList) {
        if (srv == server) return true;
    }
    return false;
}

int DapServersLocalStorage::_loadServers()
{
    QFile jsonFile(_serversListFilePath);
    if(!jsonFile.open(QFile::ReadOnly)) {
        qWarning() << "Can't open servers list file"
                   << "filepath:" << _serversListFilePath;
        return 1;
    }


    QByteArray jsBa(jsonFile.readAll());
    if(jsBa.isEmpty()) {
        qWarning() << "Serves list file is empty";
        return 2;
    }

    auto jsonDoc = QJsonDocument::fromJson(jsBa);
    if(jsonDoc.isNull() || !jsonDoc.isArray()) {
        qWarning() << "Erorr parse Serves list file to JSON"
                   << jsonDoc.isNull() << jsonDoc.isArray();
        return 3;
    }

    _jsonServersListArr = jsonDoc.array();
    _serversList.clear();

    if(DapServerInfo::fromJSON(_jsonServersListArr, _serversList) != true) {
        qCritical() << "Error parse servers list to storage";
    }
    
#ifdef  QT_DEBUG
    DapServerInfo dServerInfo;
    dServerInfo.setAddress ("127.0.0.1");
    dServerInfo.setPort (8099);
    dServerInfo.setName ("local");
    addServer(dServerInfo);
#endif
    qDebug() << "Loaded" << _jsonServersListArr.size() <<
                (_jsonServersListArr.size() == 1 ? "server" : "servers");
    return 0;
}

int DapServersLocalStorage::addServer(const DapServerInfo& server)
{
    if(isServerExists(server) == true) {
        qDebug() << server << "already exists";
        return 21;
    }

    auto jsonObj = DapServerInfo::toJSON(server);
    _jsonServersListArr.append(jsonObj);
    _serversList.append(server);

    emit changed();
    return _rewriteJsonFile();
}
