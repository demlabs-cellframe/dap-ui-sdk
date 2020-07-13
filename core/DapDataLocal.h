#pragma once

#include <QList>
#include <QString>
#include <QObject>
#include <QMap>
#include <QPair>
#include <QSettings>
#include "DapServerInfo.h"
#include "DapKeyAes.h"
#include "DapBugReportData.h"

#define SERVER_LIST_FILE "vpn-servers.xml"

class DapDataLocal : public QObject
{
    Q_OBJECT
    DapDataLocal();
    const QString ServerListName;

    QString     m_brandName;
    QString     logFilePath;
    static DapDataLocal *_me;
    static QMap<DapServerLocation, QString> m_pictruePath;

    QList<DapServerInfo> m_servers;

    void parseXML(const QString& a_fname);

    DapKeyAes *secretKey = Q_NULLPTR;
    bool initSecretKey();
    QString getRandomString(int);
    DapServerInfo* m_currentServer = nullptr;


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

    void setLogFilePath(QString path){logFilePath = path;}
    QString getLogFilePath(){return logFilePath;}

    QString serialKey() const;

    QString password() const;
    QString currentServerName() const;
    QString getServerNameByAddress(const QString& address);

    const QList<QString> &cdbServersList() { return m_cdbServersList; }
    const QString & networkDefault()       { return m_networkDefault; }
    const QString & getUrlForSignUp()      { return urlSignUp;        }
    const QString & getBrandName()         { return m_brandName;      }

    void connectComboBox(QObject *a_comboBox);

    void clearServerList();

    void saveSecretString(QString, QString);
    QString getSecretString(QString);

    static QVariant getSetting (const QString& a_setting);
    static void     saveSetting(const QString& a_setting, const QVariant& a_value);

    DapBugReportData *bugReportData();

    const QString TEXT_SERIAL_KEY   = "serialkey";
    const QString TEXT_LOGIN        = "login";
    const QString TEXT_PASSWORD     = "password";


public slots:
    void setCurrentServer(int a_serverIndex);
    void setCurrentServer(DapServerInfo* a_server);
    void setServerName(const QString &serverName);

    void setLogin(const QString &a_login);

    void setSerialKey(const QString &a_serialKey);
    void setPassword(const QString &password);

    void saveAuthorizationDatas();

    void rotateCDBList();
signals:
    /// Signal emitted if login has changed.
    /// @param login Login.
    void loginChanged(const QString& login);
    /// Signal emitted if password has changed.
    /// @param password Password.
    void passwordChanged(const QString& password);
    /// Signal emitted if password has changed.
    /// @param password Password.
    void serialKeyChanged(const QString& serial);
    /// Signal emitted if server name has changed.
    /// @param serverName Server name.
    void serverNameChanged(const QString& serverName);

    void serverAdded(const DapServerInfo& dsi);

    void serversCleared();


protected:
    QString m_login;      ///< Login.
    QString m_password;   ///< Password.
    QString m_serverName; ///< Server name.

    QString m_serialKey;  ///< Serial key.

    DapBugReportData m_bugReportData;

    QList<QString> m_cdbServersList;
    QString     m_networkDefault;
    QString urlSignUp;

private:
    void loadAuthorizationDatas();


};
