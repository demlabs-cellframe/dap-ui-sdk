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
const QString TEXT_KEY                  = "key";

class DapSerialKeyData;

/// Configuration data loaded from xml
struct ConfigData
{
    QList<QString>  m_cdbServersList;
    QString         m_networkDefault;
    QString         m_urlSite;
    QString         m_brandName;
    /// loading data from xml file
    void parseXML(const QString& a_fname);
    /// export configuration data from json object
    /// used in conjunction with fromJson
    QJsonObject             toJson();
    /// import configuration data from json object
    /// used in conjunction with toJson
    void                    fromJson(QJsonObject* data);
};

/// settings control class
class DapDataSettings : public QObject
{
    Q_OBJECT
public:
    DapDataSettings(): QObject() {}
    /// map to json object, used to pack settning data to json
    static QJsonObject  toJson(QMap<QString, QVariant> a_data);
    /// key list to json object, used to pack key list to json
    static QJsonArray   toJson(QStringList a_keys);
    /// pack settings data to json value
    /// used in conjunction with unpackItem
    static QJsonValue   packItem(const QVariant& data);
    /// unpack settings data from json value
    /// used in conjunction with packItem
    static QVariant     unpackItem(const QJsonValue& data);
    /// compare settings data
    static bool         itemCompare(const QVariant& a, const QVariant& b);
    /// get settings data by key
    virtual QVariant            getSetting (const QString& a_key) {}
    /// save settings data
    virtual void                saveSetting(const QString& a_key, const QVariant& a_value) {}
    /// remove settings data by key
    virtual void                removeSetting(const QString& a_key) {}
    /// import setting data from json object
    /// used in conjunction with toJson
    virtual void                fromJson(QJsonObject* jdata) {}
    /// export setting data from json object
    /// used in conjunction with fromJson
    virtual QJsonObject         toJson() {}
};

/// service settings control class
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
    /// Signal emitted if setting changed
    void dataUpdated(const QMap<QString, QVariant> data);
    /// Signal emitted if setting key removed
    void dataRemoved(const QStringList& keys);
private:
    QMap<QString, QVariant>  m_localData;
};

/// local settings control class
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
    /// init DapDataLocal for gui
    void initGuiData();
    /// init DapDataLocal for service
    void initServiceData();

private:
    const QString ServerListName;

    QString     m_brandName;
    QString     logFilePath;

    /// secretKey methods
private:
    DapKey *secretKey;
    void initSecretKey();
public:
    QString getRandomString(int);
    void updateSettingWithNewSecretKey();
    bool compareLocalAndServiceSecretKeys();
    bool isServiceSecretKeyEmpty();
    void saveLocalSecretKeyToService();

    static DapDataLocal* instance();
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
    /// returns a DapDataSettings pointer by key name
    DapDataSettings* settings(const QString& keyName);

public slots:
    void setLogin(const QString &a_login);
    void setPassword(const QString &password);
    void saveAuthorizationData();
    /// load config data from json object, used to load from service
    void configFromJson(QJsonObject* data)  { config.fromJson(data); }
    /// load setting data from json object, used to load from service
    void settingsFromJson(QJsonObject* data) {
        m_serviceSettings->fromJson(data);
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
    /// Signal emitted if setting changed
    void dataUpdated(const QMap<QString, QVariant> data);
    /// Signal emitted if setting key removed
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

    DapDataSettingsLocal* m_localSettings;      /// local settings management class instance
    DapDataSettingsMap* m_serviceSettings;      /// service settings management class instance

    QStringList m_keysForServerStorage;         /// list of settings keys located on the service
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
