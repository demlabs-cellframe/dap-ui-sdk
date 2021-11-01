#pragma once

#include <QList>
#include <QString>
#include <QObject>
#include <QMap>
#include <QPair>
#include <QString>
#include <QSettings>
#include <QDateTime>
#include "DapServerInfo.h"
#include "DapKeyCommon.h"
#include "DapBugReportData.h"
#include "DapServersData.h"
#include "DapSignUpData.h"
#include "DapUtils.h"
#include "DataToUpdate.h"

#define SERVER_LIST_FILE "vpn-servers.xml"

class DapSerialKeyData;

class DapDataLocal : public QObject
{
    Q_OBJECT
    DapDataLocal();
    const QString ServerListName;

    QString     m_brandName;
    QString     logFilePath;
    static QMap<DapServerLocation, QString> m_pictruePath;

    void parseXML(const QString& a_fname);

    DapKey *secretKey = Q_NULLPTR;
    void initSecretKey();
    QString getRandomString(int);

public:
    using picturesMap = QMap<DapServerLocation, QString>;
    static DapDataLocal* instance();

    QString locationToIconPath(DapServerLocation loc);

    QString login() const;

    void setLogFilePath(QString path){logFilePath = path;}
    QString getLogFilePath(){return logFilePath;}

    QString password() const;

    DataToUpdate& getDataToUpdate(){return m_dataToUpdate;}

    const QList<QString> &cdbServersList() { return m_cdbServersList; }
    const QString & networkDefault()       { return m_networkDefault; }
    const QString & getUrlSite()           { return m_urlSite;        }
    const QString & getBrandName()         { return m_brandName;      }

    QList<QString>::const_iterator m_cdbIter;

    void saveEncriptedSetting(const QString &a_setting, const QVariant &a_value);
    void saveEncriptedSetting(const QString &a_setting, const QByteArray &a_value);
    QVariant getEncriptedSetting(const QString &a_setting);
    bool loadEncriptedSettingString(const QString &a_setting, QByteArray& a_outString);

    template<typename T>
    void saveToSettings(const QString &a_setting, const T& a_value);
    template<typename T>
    bool loadFromSettings(const QString &a_setting, T& a_value);

    static QVariant getSetting (const QString& a_setting);
    static void     saveSetting(const QString& a_setting, const QVariant& a_value);
    static void     removeSetting(const QString& a_setting);
    void saveSerialKeyData();

    void saveSerialKeyDataHistory();
    QList<QString> getSerialKeyDataHistory();

    static DapBugReportData *bugReportData();
    static DapServersData   *serversData();
    DapSerialKeyData* serialKeyData();

    const QString TEXT_SERIAL_KEY           = "serialkey";
    const QString TEXT_SERIAL_KEY_HISTORY   = "serialkeyhistory";
    const QString TEXT_LOGIN                = "login";
    const QString TEXT_PASSWORD             = "password";

public slots:
    void setLogin(const QString &a_login);
    void setPassword(const QString &password);
    void saveAuthorizationData();
signals:
    /// Signal emitted if login has changed.
    /// @param login Login.
    void loginChanged(const QString& login);
    /// Signal emitted if password has changed.
    /// @param password Password.
    void passwordChanged(const QString& password);

    void licenseTermTillChanged(const QString &a_date);

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

    DataToUpdate m_dataToUpdate; ///data to update

    DapSerialKeyData* m_serialKeyData;
    QSet <QString> * m_serialKeyDataList;
};

template<typename T>
bool DapDataLocal::loadFromSettings(const QString &a_setting, T &a_value)
{
    QByteArray stringFromSettings;

    if (!this->loadEncriptedSettingString(a_setting, stringFromSettings))
        return false;

    a_value = DapUtils::fromByteArray<T>(stringFromSettings);
    return true;
}

template<typename T>
void DapDataLocal::saveToSettings(const QString &a_setting, const T &a_value)
{
    this->saveEncriptedSetting(a_setting, DapUtils::toByteArray(a_value));
}
