#pragma once

#include <QList>
#include <QString>
#include <QObject>
#include <QMap>
#include <QPair>
#include <QSettings>
#include "DapServerInfo.h"
#include "DapKeyAes.h"

#define SERVER_LIST_FILE "vpn-servers.xml"

class DapDataLocal : public QObject
{
    Q_OBJECT
    DapDataLocal();
    const QString ServerListName;
protected:
    QString     mLogin;      ///< Login.
    QString     mPassword;   ///< Password.
    QString     mServerName; ///< Server name.

    QList<QString> m_cdbServersList;
    QString     m_networkDefault;
    QString urlSignUp;

public:
    using picturesMap = QMap<DapServerLocation, QString>;
    static DapDataLocal * me(){ return _me?_me: _me = new DapDataLocal();}

    void addServer(DapServerLocation location, const QString& name,
                   const QString & address, quint16 port);
    void addServer(const DapServerInfo& dsi);
    QList<DapServerInfo>& servers(){return m_servers;}
    const DapServerInfo& serverTheBest(){ return m_servers.at(0) ;  }
    void setServerTheBest(const DapServerInfo& server);

    DapServerInfo* currentServer();
    void setRandomServerIfIsEmpty();

    QString locationToIconPath(DapServerLocation loc);

    QString login() const;
public slots:
    void setCurrentServer(int a_serverIndex);
    void setLogin(const QString &login);
public:

    QString password() const;
public slots:
    void setPassword(const QString &password);
public:

    QString serverName() const;
    QString getServerNameByAddress(const QString& address);

    const QList<QString> &cdbServersList() { return  m_cdbServersList; }
    const QString & networkDefault() { return  m_networkDefault; }
    const QString & getUrlForSignUp() { return  urlSignUp; }

    void connectComboBox(QObject *a_comboBox);

    void clearServerList();

    void saveSecretString(QString, QString);
    QString getSecretString(QString);


public slots:
    void setServerName(const QString &serverName);

private:
    static DapDataLocal *_me;
    static QMap<DapServerLocation, QString> m_pictruePath;

    QList<DapServerInfo> m_servers;

    void parseXML(const QString& a_fname);

    DapKeyAes *secretKey = Q_NULLPTR;
    bool initSecretKey();
    QString getRandomString(int);

signals:
    /// Signal emitted if login has changed.
    /// @param login Login.
    void loginChanged(const QString& login);
    /// Signal emitted if password has changed.
    /// @param password Password.
    void passwordChanged(const QString& password);
    /// Signal emitted if server name has changed.
    /// @param serverName Server name.
    void serverNameChanged(const QString& serverName);

    void serverAdded(const DapServerInfo& dsi);

    void serversCleared();

private:

    DapServerInfo* m_currentServer = nullptr;
};
