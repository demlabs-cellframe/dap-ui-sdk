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
#include "DapServerList.h"
#include "DapBugReportHistory.h"
#include "DapSignUpData.h"
#include "DapUtils.h"
#include "DataToUpdate.h"

#define SERVER_LIST_FILE "vpn-servers.xml"

const QString TEXT_SERIAL_KEY           = "serialkey";
const QString TEXT_SERIAL_KEY_HISTORY   = "serialkeyhistory";
const QString TEXT_PENDING_SERIAL_KEY   = "pendingserialkey";
const QString TEXT_BUGREPORT_HISTORY    = "bugreporthistory";
const QString ROUTING_EXCEPTIONS_LIST   = "routingexceptionslist";
const QString TEXT_LOGIN                = "login";
const QString TEXT_PASSWORD             = "password";
const QString TEXT_TX_OUT               = "tx_out";
const QString COUNTRY_NAME              = "Country_code";
const QString SETTING_AUTHORIZATION     = "authorization";
const QString LAST_SELECTED_SERVER      = "lastSelectedServer";
const QString SETTING_THEME             = "ColorTheme";
const QString SETTING_ROUEXC_MODE       = "rouExcMode";

class DapSerialKeyData;
enum class Authorization;
class DapSerialKeyHistory;

class DapCdbServer
{
  /* variables */
public:
  QString address;
  int port;

  /* methods */
public:
  QString toString() const;
  void fromString (const QString &a_src);
  static DapCdbServer serverFromString (const QString &a_src);
};

class DapCdbServerList : public QList<DapCdbServer>
{
  /* methods */
public:
  static DapCdbServerList toServers (const QStringList &a_src);
  static QStringList toStrings (const DapCdbServerList &a_servers);
};

class DapDataLocal : public QObject
{
public:
    Q_OBJECT
    DapDataLocal();
    const QString ServerListName;

    QString     m_brandName;
    QString     logFilePath;
    QString     m_logPath;

    void parseXML(const QString& a_fname);

    DapKey *secretKey = Q_NULLPTR;
    void initSecretKey();
    QString getRandomString(int);

public:
    static DapDataLocal* instance();

    QString login() const;

    void setLogFilePath(QString path){logFilePath = path;}
    QString getLogFilePath(){return logFilePath;}
    void setLogPath(QString path){m_logPath = path;}
    QString getLogPath(){return m_logPath;}


    QString password() const;

    DataToUpdate& getDataToUpdate(){return m_dataToUpdate;}

    const DapCdbServerList &cdbServersList()  { return m_cdbServersList; }
    const QString & networkDefault()          { return m_networkDefault; }
    const QString & getUrlSite()              { return m_urlSite;        }
    const QString & getBrandName()            { return m_brandName;      }

    DapCdbServerList::const_iterator m_cdbIter;

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

    //void saveHistoryData(QString a_type, QString a_data);
    //QList<QString> getHistorySerialKeyData();
    //void removeItemFromHistory(QString a_type, QString a_item);

    static DapBugReportData *bugReportData();
    DapSerialKeyData* serialKeyData();

    QString pendingSerialKey(){return m_pendingSerialKey;};

    DapBugReportHistory *bugReportHistory();
    DapSerialKeyHistory *serialKeyHistory();

    Authorization authorizationType();
    void setAuthorizationType(Authorization type);

    void updateCdbList (const DapCdbServerList &a_newCdbList);

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

    //void sigHistoryDataSaved(const QString &type);

protected:
    DapCdbServerList  m_cdbServersList;
    QString           m_networkDefault;
    QString           m_urlSite;

private:
    void loadAuthorizationDatas();
    void _syncCdbWithSettings();
    static QSettings* settings();

    QString m_login;      ///< Login.
    QString m_password;   ///< Password.
    QString m_serialKey;  ///< Serial key.

    DataToUpdate m_dataToUpdate; ///data to update

    DapSerialKeyData* m_serialKeyData;
    QSet <QString> * m_serialKeyDataList;
    QString m_pendingSerialKey;

    DapBugReportHistory *m_bugReportHistory;
    DapSerialKeyHistory *m_serialKeyHistory;
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

enum class Authorization
{
    account,
    serialKey,
    certificate,
    undefined
};
