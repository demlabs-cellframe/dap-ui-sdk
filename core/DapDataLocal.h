#ifndef DAPDATALOCAL_H
#define DAPDATALOCAL_H

/* INCLUDES */

#include "DapServerInfo.h"
#include "DapKeyCommon.h"
#include "DapBugReportData.h"
#include "DapServerList.h"
#include "DapBugReportHistory.h"
#include "DapSignUpData.h"
#include "DapUtils.h"
#include "DapDataConfig.h"

#include <QList>
#include <QString>
#include <QObject>
#include <QMap>
#include <QPair>
#include <QString>
#include <QDateTime>

/* DEFINES */

#define SERVER_LIST_FILE "vpn-servers.xml"

class DapSerialKeyData;
class DapSerialKeyHistory;

enum class Authorization
{
  account,
  serialKey,
  certificate,
  undefined
};

/* VARIABLES */

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
const QString LAST_SELECTED_SERVER_HASH = "lastSelectedServerOrderHash";
const QString SETTING_THEME             = "ColorTheme";
const QString SETTING_ROUEXC_MODE       = "rouExcMode";
const QString NODE_ORDER_HISTORY        = "nodeOrderHistory";
const QString SERIAL_LOSS_ON_UNINSTALL_FLAG = "serialLossOnUninstallFlag";
const QString NOTIFICATION_HISTORY      = "notificationHistory";
const QString NOCDB_DATA                = "NoCdbData";
const QString COUNTRY_ISO               = "country_iso";

/****************************************//**
 * @brief CDB Server
 * @date 10.11.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapCdbServer
{
  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
public:
  QString address;
  int port;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString toString() const;
  void fromString (const QString &a_src);

  static DapCdbServer serverFromString (const QString &a_src);
  /// @}
};

/****************************************//**
 * @brief CDB Server List
 * @date 10.11.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapCdbServerList : public QList<DapCdbServer>
{
  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapCdbServerList toServers (const QStringList &a_src);
  static QStringList toStrings (const DapCdbServerList &a_servers);
  /// @}
};

/****************************************//**
 * @brief Data Local
 * @date 03.11.2019
 * @author Dmitriy Gerasimov, Mikhail Shilenko
 *******************************************/

class DapDataLocal : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFINES
   *******************************************/
  /// @{
  enum class UpdateSide : quint8
  {
    service,
    gui,
    tray,
    cli,
  };
  Q_ENUM(UpdateSide)
  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  DapCdbServerList m_cdbServersList;
  QString m_kelvpnPub;
  QString m_networkDefault;
  QString m_urlSite;

  QString m_coutryISO;

  QString m_login;      ///< Login.
  QString m_password;   ///< Password.

  QString m_brandName;
  QString m_logFilePath;
  QString m_logPath;
  QString m_minDashboardVersion;
  QString m_minNodeVersion;

  DapKey *secretKey = Q_NULLPTR;

  DapSerialKeyData *m_serialKeyData;
  QSet<QString> *m_serialKeyDataList;
  QString m_pendingSerialKey;

  DapBugReportHistory *m_bugReportHistory;
  DapSerialKeyHistory *m_serialKeyHistory;

public:
  DapCdbServerList::const_iterator m_cdbIter;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapDataLocal();
  ~DapDataLocal() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapDataLocal* instance();

  /* AUTH */

  QString login() const;
  void setLogin (const QString &a_login);

  QString password() const;
  void setPassword (const QString &a_password);

  Authorization authorizationType() const;
  void setAuthorizationType (Authorization a_type);

  void saveAuthorizationData();

  /* LOG */

  void setLogFilePath (const QString &a_path);
  const QString &getLogFilePath();

  void setLogPath (const QString &a_path);
  const QString &getLogPath();

  /* FIELDS */

  const DapCdbServerList &cdbServersList() const;
  const QString &KelvpnPub() const;
  const QString &networkDefault() const;
  const QString &getUrlSite() const;
  const QString &getBrandName() const;

  const QString &getMinDashboardVersion() const;
  const QString &getMinNodeVersion() const;

  const QString &getCountryISO() const;
  void setCountryISO (const QString &a_iso_code);

  DapSerialKeyData* serialKeyData();
  DapBugReportHistory *bugReportHistory();
  DapSerialKeyHistory *serialKeyHistory();

  static DapBugReportData *bugReportData();

  virtual DapDataConfig *settings() = 0;

  /* SETTINGS */

  void saveEncryptedSetting (const QString &a_setting, const QVariant &a_value);
  void saveEncryptedSetting (const QString &a_setting, const QByteArray &a_value);
  QVariant getEncryptedSetting (const QString &a_setting) const;
  bool loadEncryptedSettingString (const QString &a_setting, QByteArray &a_outString) const;

  template<typename T>
  void saveToSettings (const QString &a_setting, const T& a_value);
  template<typename T>
  bool loadFromSettings (const QString &a_setting, T& a_value);

  static QVariant getSetting (const QString& a_setting);
  static void saveSetting (const QString& a_setting, const QVariant& a_value);
  static void removeSetting (const QString& a_setting);

  /* SERIAL KEY */

  void saveSerialKeyData();
  void resetSerialKeyData();

  void savePendingSerialKey (const QString &a_serialkey);
  const QString &pendingSerialKey() const;

  /* CDB */

  void updateCdbList (const DapCdbServerList &a_newCdbList);

  /* STATIC */

  static QString getRandomString (int a_size);

  /* PROTECTED */
protected:
  void _parseXML (const QString& a_fname);
  void _initSecretKey();
  void _loadAuthorizationDatas();
  void _syncCdbWithSettings();
  //static QSettings *settings();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigLoginChanged (QString a_login);
  void sigPasswordChanged (QString a_password);
  /// @}
};

/********************************************
 * METHODS
 *******************************************/

template<typename T>
bool DapDataLocal::loadFromSettings (const QString &a_setting, T &a_value)
{
  QByteArray stringFromSettings;

  if (!loadEncryptedSettingString (a_setting, stringFromSettings))
    return false;

  a_value = DapUtils::fromByteArray<T> (stringFromSettings);
  return true;
}

template<typename T>
void DapDataLocal::saveToSettings (const QString &a_setting, const T &a_value)
{
  saveEncryptedSetting (a_setting, DapUtils::toByteArray (a_value));
}

/*-----------------------------------------*/
#endif // DAPDATALOCAL_H
