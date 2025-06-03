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
#include "DapServerList.h"
#include "DapBugReportHistory.h"
#include "DapUtils.h"
#include "DataToUpdate.h"

#define SERVER_LIST_FILE "vpn-servers.xml"


class DapSerialKeyData;
enum class Authorization;
class DapSerialKeyHistory;

class DapCdbServer
{
public:
    QString address;
    int port;
public:
    QString toString() const;
    void fromString (const QString &a_src);
    static DapCdbServer serverFromString (const QString &a_src);
};

class DapCdbServerList : public QList<DapCdbServer>
{
public:
    static DapCdbServerList toServers (const QStringList &a_src);
    static QStringList toStrings (const DapCdbServerList &a_servers);
};

class DapBaseDataLocal : public QObject
{
    Q_OBJECT
public:
    using CdbIterator = DapCdbServerList::const_iterator;

    static const QString TEXT_SERIAL_KEY;
    static const QString TEXT_SERIAL_KEY_HISTORY;
    static const QString TEXT_PENDING_SERIAL_KEY;
    static const QString UPDATE_FILE_PATH;
    static const QString TEXT_BUGREPORT_HISTORY;
    static const QString ROUTING_EXCEPTIONS_LIST;
    static const QString TEXT_LOGIN;
    static const QString TEXT_PASSWORD;
    static const QString TEXT_TX_OUT;
    static const QString COUNTRY_NAME;
    static const QString SETTING_AUTHORIZATION;
    static const QString LAST_SELECTED_SERVER;
    static const QString LAST_SELECTED_SERVER_HASH;
    static const QString SETTING_THEME;
    static const QString SETTING_ROUEXC_MODE;
    static const QString NODE_ORDER_HISTORY;
    static const QString SERIAL_LOSS_ON_UNINSTALL_FLAG;
    static const QString NOTIFICATION_HISTORY;
    static const QString NOCDB_DATA;
    static const QString COUNTRY_ISO;
    static const QString SETTING_LOCALE;
    static const QString SETTING_SYS_LOCALE;
    static const QString AGREEMENT_CHECKED;
    static const QString LAST_NODE_LIST_UPDATE;
    static const QString LAST_NODE_LIST_UPDATE_TIME;
    static const QString MIGRATION_KEY;

    DapBaseDataLocal();

    void parseXML(const QString& a_fname);

    void initSecretKey();
    QString getRandomString(int);

    QSettings* settings() { return m_settings; }
    QString login() const;

    QString getPub() {return m_pubStage;}
    const CdbIterator& getCdbIterator() const {return m_cdbIter;}

    QString password() const;

    DataToUpdate& getDataToUpdate(){return m_dataToUpdate;}

    void setNewCbdIterator(const CdbIterator& iterator){ m_cdbIter = iterator; }
    void nextCbdIterator() {m_cdbIter++;}
    const DapCdbServerList &cdbServersList()  { return m_cdbServersList; }
    const QString & KelvpnPub()               { return m_kelvpnPub;}
    const QString & networkDefault()          { return m_networkDefault; }

    const QString & getMinDashboardVersion() const { return m_minDashboardVersion; }
    const QString & getMinNodeVersion()      const { return m_minNodeVersion; }

    const QString & getCountryISO() { return m_coutryISO; }
    virtual void setCountryISO(const QString& iso_code){ m_coutryISO = iso_code;}

    virtual void saveKeysHistory();
    virtual void saveEncryptedSetting(const QString &a_setting, const QVariant &a_value);
    virtual void saveEncryptedSetting(const QString &a_setting, const QByteArray &a_value);
    QVariant getEncryptedSetting(const QString &a_setting);
    virtual bool loadEncryptedSettingString(const QString &a_setting, QByteArray& a_outString);

    template<typename T>
    void saveToSettings(const QString &a_setting, const T& a_value);
    template<typename T>
    bool loadFromSettings(const QString &a_setting, T& a_value);

    virtual QVariant getValueSetting(const QString& a_setting);
    virtual void saveValueSetting(const QString& a_setting, const QVariant& a_value);
    virtual void removeValueSetting(const QString& a_setting);
    virtual void setSettings(const QJsonObject &json);

    virtual void saveSerialKeyData();
    virtual void resetSerialKeyData();

    virtual void savePendingSerialKey(QString a_serialkey);

    DapSerialKeyData* serialKeyData();

    QString pendingSerialKey(){return m_pendingSerialKey;};

    DapBugReportHistory *bugReportHistory();
    DapSerialKeyHistory *serialKeyHistory();

    Authorization authorizationType();
    void setAuthorizationType(Authorization type);

    virtual void updateCdbList (const DapCdbServerList &a_newCdbList);

    QJsonObject toJson();
    virtual void fromJson (const QJsonObject &json);

    virtual void loadBugReport();
    virtual void saveBugReport();

    const QJsonArray cbdServerListToJson() const;
    const QJsonObject dataToUpdateToJson() const;
    const QJsonObject serialKeyDataToJson() const;
    const QJsonArray serialKeyDataListToJson() const;
    const QJsonArray bugReportHistoryToJson() const;
    const QJsonArray serialKeyHistoryToJson() const;
    const QJsonObject settingsToJson();

    void saveMigrate();
public slots:
    virtual void setLogin(const QString &a_login);
    virtual void setPassword(const QString &password);
    virtual void saveAuthorizationData();

    virtual void addNewSerialKey();
signals:
    /// Signal emitted if login has changed.
    /// @param login Login.
    void loginChanged(const QString& login);
    /// Signal emitted if password has changed.
    /// @param password Password.
    void passwordChanged(const QString& password);

    void licenseTermTillChanged(const QString &a_date);

    void serialKeyAdded();
protected:
    void loadKeysHistory();
    void initData();
    void initAuthData();

    void loadAuthorizationDatas();
    void syncCdbWithSettings();
    void initSettings(const QString &path = "");


    void jsonToValue(QString& str, const QJsonObject& object, const QString& key);
    void jsonToValue(bool &data, const QJsonObject& object, const QString& key);
    void jsonToValue(int &data, const QJsonObject& object, const QString& key);
    QJsonObject createJsonObject(const QString& itemName, const QString& itemValue);


    void setSbdServerList(const QJsonArray& list);
    void setDataToUpdate(const QJsonObject& object);
    void setSerialKeyData(const QJsonObject& object);
    void setSerialKeyDataList(const QJsonArray& list);
    void setSerialKeyHistory(const QJsonArray& list);

    virtual void setBugReportHistory(const QJsonArray& list);

    template<typename T>
    void saveToSettingsBase(const QString &a_setting, const T& a_value);
    template<typename T>
    bool loadFromSettingsBase(const QString &a_setting, T& a_value);
protected:
    DapCdbServerList::const_iterator m_cdbIter;

    DapCdbServerList  m_cdbServersList;

    QString m_kelvpnPub;
    QString m_networkDefault;
    QString m_urlSite;
    QString m_coutryISO;
    QString m_login;      ///< Login.
    QString m_password;   ///< Password.
    QString m_minDashboardVersion;
    QString m_minNodeVersion;
    QString m_pubStage;
    QString m_pendingSerialKey;
    QString m_brandName;

    DapKey *secretKey = Q_NULLPTR;

    DataToUpdate m_dataToUpdate; ///data to update

    DapSerialKeyData* m_serialKeyData;
    QSet<QString> m_serialKeyDataList;

    DapBugReportHistory *m_bugReportHistory;
    DapSerialKeyHistory *m_serialKeyHistory;

    QSettings* m_settings = nullptr;

    const QString JSON_CBD_SERVERS_KEY = "cbdServers";
    const QString JSON_PORT_KEY = "port";
    const QString JSON_ADDRESS_KEY = "address";
    const QString JSON_KELVPN_PUB_KEY = "kelvpnPub";
    const QString JSON_NETWORK_DEFAULT_KEY = "networkDefault";
    const QString JSON_URL_SITE_KEY = "urlSite";
    const QString JSON_COUNTRY_ISO_KEY = "coutryISO";
    const QString JSON_LOGIN_KEY = "login";
    const QString JSON_PASSWORD_KEY = "password";
    const QString JSON_SERIAL_KEY_KEY = "serialKey";
    const QString JSON_MIN_DASHBOARD_VERSION_KEY = "minDashboardVersion";
    const QString JSON_MIN_NODE_VERSION_KEY = "minNodeVersion";
    const QString JSON_PUB_STAGE_KEY = "pubStage";
    const QString JSON_PENDING_SERIAL_KEY_KEY = "pendingSerialKey";
    const QString JSON_STATE_KEY = "state";
    const QString JSON_VERSION_KEY = "version";
    const QString JSON_URL_UPDATE_KEY = "URLUpdate";
    const QString JSON_SHOW_MESSAGE_KEY = "showMessage";
    const QString JSON_FORCE_DOWNLOAD_KEY = "forceDownload";
    const QString JSON_FORCE_INSTALL_KEY = "forceInstall";
    const QString JSON_DATA_TO_UPDATE_KEY = "dataToUpdate";
    const QString JSON_IS_ACTIVATED_KEY = "isActivated";
    const QString JSON_LISENSE_TIME_KEY = "lisenseTime";
    const QString JSON_DAYS_LEFT_STRING_KEY = "daysLeftString";
    const QString JSON_SERIAL_KEY_DATA_KEY = "serialKeyData";
    const QString JSON_SERIAL_KEY_DATA_LIST_KEY = "serialKeyDataList";
    const QString JSON_BUG_NUMBER_KEY = "bugNumber";
    const QString JSON_NUMBER_KEY = "number";
    const QString JSON_STATUS_KEY = "status";
    const QString JSON_BUG_REPORT_HISTORY_KEY = "bugReportHistory";
    const QString JSON_SERIAL_KEY_HISTORY_KEY = "serialKeyHistory";
    const QString JSON_SETTINGS_KEY = "settings";
};

template<typename T>
bool DapBaseDataLocal::loadFromSettings(const QString &a_setting, T &a_value)
{
    QByteArray stringFromSettings;

    if (!this->loadEncryptedSettingString(a_setting, stringFromSettings))
        return false;

    a_value = DapUtils::fromByteArray<T>(stringFromSettings);
    return true;
}

template<typename T>
void DapBaseDataLocal::saveToSettings(const QString &a_setting, const T &a_value)
{
    this->saveEncryptedSetting(a_setting, DapUtils::toByteArray(a_value));
}

template<typename T>
void DapBaseDataLocal::saveToSettingsBase(const QString &a_setting, const T &a_value)
{
    QByteArray value = DapUtils::toByteArray(a_value);
    QByteArray encodedString;
    secretKey->encode(value, encodedString);
    saveValueSetting(a_setting, encodedString);
}

template<typename T>
bool DapBaseDataLocal::loadFromSettingsBase(const QString &a_setting, T &a_value)
{
    QByteArray stringFromSettings;

    auto load = [this](const QString &a_setting, QByteArray& a_outString) -> bool
    {
        QVariant varSettings = settings()->value(a_setting);;

        if (!varSettings.isValid() || !varSettings.canConvert<QByteArray>())
            return false;

        QByteArray encryptedString = varSettings.toByteArray();
        if (encryptedString.isEmpty())
        {
            a_outString = "";
            return true;
        }
        secretKey->decode(encryptedString, a_outString);

        return true;
    };

    if (!load(a_setting, stringFromSettings))
        return false;

    a_value = DapUtils::fromByteArray<T>(stringFromSettings);
    return true;
}


enum class Authorization
{
    account,
    serialKey,
    certificate,
    undefined
};
