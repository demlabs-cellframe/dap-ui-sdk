#ifndef DAPSERVERINFO_H
#define DAPSERVERINFO_H

/* INCLUDES */
#include <QString>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

/* DEFS */
class DapServerInfo;
using DapServerInfoList = QVector<DapServerInfo>;

//#define DAP_CONNECTION_QUALITY DapServerInfo::ConnectionQuality

/****************************************//**
 * @brief Dap Server Info
 *******************************************/

class DapServerInfo
{
  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum class ConnectionQuality : qint16
  {
    INVALID = -1,
    NO_CONNECTION,
    FULL,
    ALMOST_FULL,
    MIDDLE,
    LOW,
    VERY_LOW
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QString m_address;
  QString m_address6;
  quint16 m_port;
  QString m_name;
  QString m_location;
  QString m_online;
  int m_ping;
  ConnectionQuality m_connQuality;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapServerInfo();
  DapServerInfo (const QJsonObject &a_src);
  DapServerInfo (const DapServerInfo &a_src);
  DapServerInfo (DapServerInfo &&a_src);
  DapServerInfo (
    QString a_location,
    QString a_name,
    QString a_address,
    quint16 a_port
  );
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static bool fromJSON (const QJsonArray &jsonArr, DapServerInfoList &out);
  static bool fromJSON (const QJsonObject &jsonObj, DapServerInfo &out);
  static QJsonObject toJSON (const DapServerInfo &dsi);
  QJsonObject toJSON() const;
  /// @deprecated
  static void sortServerList (QList<DapServerInfo> &serverList);
  /// @deprecated
  static void addGeneralLocation (
    const QList<DapServerInfo> &serverList,
    QList<DapServerInfo> &generalServerList,
    QString base_location);

  const QString &address() const;
  void    setAddress (const QString &address);

  const QString &address6() const;
  void    setAddress6 (const QString &address6);

  quint16 port() const;
  void    setPort (const quint16 &port);

  const QString &name() const;
  void    setName (const QString &name);

  const QString &location() const;
  void    setLocation (const QString &location);

  const QString &online() const;
  void    setOnline (const QString &online);

  int     ping() const;
  void    setPing (int ping);

  ConnectionQuality connQuality() const;
  void setConnQuality (const ConnectionQuality &connQuality);

  bool isAuto() const;
  bool isValid() const;
  bool isOnline() const;

  QVariant value (int a_fieldId) const;

private:
  static bool _isJsonValid (const QJsonObject &obj);
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator= (const DapServerInfo &a_src);
  DapServerInfo &operator= (DapServerInfo &&a_src);
  friend bool operator== (const DapServerInfo &lhs, const DapServerInfo &rhs);
  friend bool operator> (const DapServerInfo &lhs, const DapServerInfo &rhs) { return lhs.m_ping > rhs.m_ping; }
  friend bool operator< (const DapServerInfo &lhs, const DapServerInfo &rhs) { return lhs.m_ping < rhs.m_ping; }
  friend QDebug operator<< (QDebug out, const DapServerInfo &dsi)
  {
    out <<  Qt::endl << " name: " << dsi.m_name << " location: "
        << dsi.m_location << " address: " << dsi.m_address <<  ":" << dsi.m_port << " state: " << dsi.m_online << " ping: " << dsi.m_ping;
    return out;
  }
};

Q_DECLARE_METATYPE (DapServerInfo)
typedef DapServerInfo *DapServerInfoPtr;
Q_DECLARE_METATYPE (DapServerInfoPtr)

/****************************************//**
 * @brief Dap Server Info Type
 *
 * Used to describe type to high level (QML)
 *******************************************/

class DapServerType : public QObject, public DapServerInfo
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum FieldId : quint16
  {
    name    = Qt::DisplayRole,
    address = Qt::UserRole,
    address6,
    port,
    location,
    online,
    ping,
    connQuality,
  };
  Q_ENUM (FieldId)
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapServerType();
  DapServerType (const DapServerInfo &a_src);
  DapServerType (DapServerType &&a_src);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
  QVariant asVariantMap() const;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPSERVERINFO_H
