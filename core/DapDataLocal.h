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
#include "DapServersData.h"
#include "DapSignUpData.h"

#define SERVER_LIST_FILE "vpn-servers.xml"

class DapDataLocal : public QObject
{
    Q_OBJECT
    DapDataLocal();
    const QString ServerListName;

    QString     m_brandName;
    QString     logFilePath;
    static QMap<DapServerLocation, QString> m_pictruePath;


    void parseXML(const QString& a_fname);

    DapKeyAes *secretKey = Q_NULLPTR;
    bool initSecretKey();
    QString getRandomString(int);

public:
    using picturesMap = QMap<DapServerLocation, QString>;
    static DapDataLocal* instance();

    QString locationToIconPath(DapServerLocation loc);

    QString login() const;

    void setLogFilePath(QString path){logFilePath = path;}
    QString getLogFilePath(){return logFilePath;}

    QString serialKey() const;

    QString password() const;

    QString urlUpdate() const;

    const QList<QString> &cdbServersList() { return m_cdbServersList; }
    const QString & networkDefault()       { return m_networkDefault; }
    const QString & getUrlSite()           { return m_urlSite;        }
    const QString & getBrandName()         { return m_brandName;      }

    void saveEncriptedSetting(const QString &a_setting, const QVariant &a_value);
    QVariant getEncriptedSetting(const QString &a_setting);

    static QVariant getSetting (const QString& a_setting);
    static void     saveSetting(const QString& a_setting, const QVariant& a_value);

    static DapBugReportData *bugReportData();
    static DapServersData   *serversData();

    const QString TEXT_SERIAL_KEY   = "serialkey";
    const QString TEXT_LOGIN        = "login";
    const QString TEXT_PASSWORD     = "password";

    QVector<DapServerInfo> m_serversForCheck;

public slots:
    void setLogin(const QString &a_login);

    void setSerialKey(const QString &a_serialKey);
    void setPassword(const QString &password);

    void setUrlUpdate(const QString &a_url);

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

protected:
    QList<QString>  m_cdbServersList;
    QString         m_networkDefault;
    QString         m_urlSite;

private:
    void loadAuthorizationDatas();
    static QSettings* settings();

    QString m_login;      ///< Login.
    QString m_password;   ///< Password.
    QString m_serialKey;  ///< Serial key.

    QString m_urlUpdate; ///< url for download
};


