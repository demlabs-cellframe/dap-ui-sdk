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
#include "DapBugReportHistory.h"
#include "DapSignUpData.h"
#include "DapUtils.h"
#include "DataToUpdate.h"

#define SERVER_LIST_FILE "vpn-servers.xml"

const QString TEXT_SERIAL_KEY           = "serialkey";
const QString TEXT_SERIAL_KEY_HISTORY   = "serialkeyhistory";
const QString TEXT_PENDING_SERIAL_KEY   = "pendingserialkey";
const QString TEXT_BUGREPORT_HISTORY    = "bugreporthistory";
const QString TEXT_LOGIN                = "login";
const QString TEXT_PASSWORD             = "password";
const QString TEXT_TX_OUT               = "tx_out";

class DapSerialKeyData;


struct ConfigData
{
    QList<QString>  m_cdbServersList;
    QString         m_networkDefault;
    QString         m_urlSite;
    QString         m_brandName;

    void parseXML(const QString& a_fname);
    QJsonObject             toJson();
    void                    fromJson(QJsonObject* data);
};


class DapDataSettings : public QObject
{
    Q_OBJECT
public:
    DapDataSettings(): QObject() {}
    static QJsonObject  toJson(QMap<QString, QVariant> a_data);
    static QJsonArray   toJson(QStringList a_keys);
    static QJsonValue   packItem(const QVariant& data);
    static QVariant     unpackItem(const QJsonValue& data);
    static bool         itemCompare(const QVariant& a, const QVariant& b);
    virtual QVariant            getSetting (const QString& a_key) {}
    virtual void                saveSetting(const QString& a_key, const QVariant& a_value) {}
    virtual void                removeSetting(const QString& a_key) {}
    virtual void                fromJson(QJsonObject* jdata) {}
    virtual QJsonObject         toJson() {}
};


class DapDataSettingsMap : public DapDataSettings
{
    Q_OBJECT
public:
    DapDataSettingsMap(): DapDataSettings() {}
    QVariant            getSetting (const QString& a_key) override;
    void                saveSetting(const QString& a_key, const QVariant& a_value) override;
    void                removeSetting(const QString& a_key) override;
    void                fromJson(QJsonObject* jdata) override;
    QJsonObject         toJson() override;
signals:
    void dataUpdated(const QMap<QString, QVariant> data);
    void dataRemoved(const QStringList& keys);
private:
    QMap<QString, QVariant>  m_localData;
};


class DapDataSettingsLocal : public DapDataSettings
{
    Q_OBJECT
public:
    DapDataSettingsLocal(): DapDataSettings() {}
    QVariant            getSetting (const QString& a_key) override;
    void                saveSetting(const QString& a_key, const QVariant& a_value) override;
    void                removeSetting(const QString& a_key) override;
    QJsonObject         toJson() override;
    QStringList         allKeys() {return settings()->allKeys();}
private:
    QSettings*          settings();
};



class DapDataLocal : public QObject
{
    Q_OBJECT
public:
    DapDataLocal();
    const QString ServerListName;

    QString     m_brandName;
    QString     logFilePath;

    DapKey *secretKey;
    void initSecretKey();
    QString getRandomString(int);

public:
    static DapDataLocal* instance();
    void parseXML(const QString& a_fname);

    QString login() const;

    void setLogFilePath(QString path){logFilePath = path;}
    QString getLogFilePath(){return logFilePath;}

    QString password() const;

    const QList<QString> &cdbServersList() { return config.m_cdbServersList; }
    const QString & networkDefault()       { return config.m_networkDefault; }
    const QString & getUrlSite()           { return config.m_urlSite;        }
    const QString & getBrandName()         { return config.m_brandName;      }

    QList<QString>::const_iterator m_cdbIter;

    void saveEncryptedSetting(const QString &a_setting, const QVariant &a_value);
    void saveEncryptedSetting(const QString &a_setting, const QByteArray &a_value);
    QVariant getEncryptedSetting(const QString &a_setting);
    bool loadEncryptedSettingString(const QString &a_setting, QByteArray& a_outString);

    template<typename T>
    void saveToSettings(const QString &a_setting, const T& a_value);
    template<typename T>
    bool loadFromSettings(const QString &a_setting, T& a_value);

    static QVariant getSetting (const QString& a_setting);
    static void     saveSetting(const QString& a_setting, const QVariant& a_value);
    static void     removeSetting(const QString& a_setting);
    void saveSerialKeyData();
    void resetSerialKeyData();

    void savePendingSerialKey(QString a_serialkey);

    void saveHistoryData(QString a_type, QString a_data);
    QList<QString> getHistorySerialKeyData();
    void removeItemFromHistory(QString a_type, QString a_item);

    static DapBugReportData *bugReportData();
    static DapServersData   *serversData();
    DapSerialKeyData* serialKeyData();

    QString pendingSerialKey(){return m_pendingSerialKey;};

    DapBugReportHistory *bugReportHistory();
    DapDataSettings* settings(const QString& keyName);

public slots:
    void setLogin(const QString &a_login);
    void setPassword(const QString &password);
    void saveAuthorizationData();
    void configFromJson(QJsonObject* data)  { config.fromJson(data); }
    void settingsFromJson(QJsonObject* data) {
        m_service_settings->fromJson(data);
        loadAuthorizationDatas();
        m_loadAuthorizationDatas = true;
    }

signals:
    /// Signal emitted if login has changed.
    /// @param login Login.
    void loginChanged(const QString& login);
    /// Signal emitted if password has changed.
    /// @param password Password.
    void passwordChanged(const QString& password);
    void licenseTermTillChanged(const QString &a_date);
    void sigHistoryDataSaved();
    /// server data setting changed
    void dataUpdated(const QMap<QString, QVariant> data);
    void dataRemoved(const QStringList& keys);

protected:
    ConfigData config;

private:
    void loadAuthorizationDatas();
    bool m_loadAuthorizationDatas;

    QString m_login;      ///< Login.
    QString m_password;   ///< Password.
    QString m_serialKey;  ///< Serial key.

    DapSerialKeyData* m_serialKeyData;
    QSet <QString> * m_serialKeyDataList;
    QString m_pendingSerialKey;

    DapBugReportHistory* m_buReportHistory;
    QMap<QString, QVariant> m_settings;
    DapDataSettingsLocal* m_local_settings;
    DapDataSettingsMap* m_service_settings;
};

template<typename T>
bool DapDataLocal::loadFromSettings(const QString &a_setting, T &a_value)
{
    QByteArray stringFromSettings;

    if (!this->loadEncryptedSettingString(a_setting, stringFromSettings))
        return false;

    a_value = DapUtils::fromByteArray<T>(stringFromSettings);
    return true;
}

template<typename T>
void DapDataLocal::saveToSettings(const QString &a_setting, const T &a_value)
{
    this->saveEncryptedSetting(a_setting, DapUtils::toByteArray(a_value));
}
