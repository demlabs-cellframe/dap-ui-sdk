#ifndef DAPNODEORDERINFO_H
#define DAPNODEORDERINFO_H

/* INCLUDES */
#include <QString>
#include <QJsonObject>

/****************************************//**
 * @brief order info class
 * @date 05.09.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapNodeOrderInfo
{
  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum class FieldId : quint8
  {
    INVALID,

    direction,
    ext,
    hash,
    nodeAddress,
    nodeLocation,
    pkey,
    price,
    priceUnit,
    srvUid,
    txCondHash,
    units,
    version,

    /* field copies to prevent parsing issues */
    // {

    node_addr,
    node_location,
    price_unit,
    srv_uid,
    tx_cond_hash,

    // }

    SIZE,
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QString m_direction;
  QString m_ext;
  QString m_hash;
  QString m_nodeAddress;
  QString m_nodeLocation;
  QString m_pkey;
  QString m_price;
  QString m_priceUnit;
  QString m_srvUid;
  QString m_txCondHash;
  QString m_units;
  QString m_version;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapNodeOrderInfo();
  DapNodeOrderInfo (const DapNodeOrderInfo &a_src);
  DapNodeOrderInfo (DapNodeOrderInfo &&a_src);
  ~DapNodeOrderInfo();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /* fields */

  const QString &direction() const;
  void setDirection (const QString &a_value);

  const QString &ext() const;
  void setExt (const QString &a_value);

  const QString &hash() const;
  void setHash (const QString &a_value);

  const QString &nodeAddress() const;
  void setNodeAddress (const QString &a_value);

  const QString &nodeLocation() const;
  void setNodeLocation (const QString &a_value);

  const QString &pkey() const;
  void setPkey (const QString &a_value);

  const QString &price() const;
  void setPrice (const QString &a_value);

  const QString &priceUnit() const;
  void setPriceUnit (const QString &a_value);

  const QString &srvUid() const;
  void setSrvUid (const QString &a_value);

  const QString &txCondHash() const;
  void setTxCondHash (const QString &a_value);

  const QString &units() const;
  void setUnits (const QString &a_value);

  const QString &version() const;
  void setVersion (const QString &a_value);

  /* data */

  const QString &value (FieldId a_fid) const;
  const QString &value (const QByteArray &a_name) const;
  void setValue (FieldId a_fid, const QString &a_value);
  void setValue (FieldId a_fid, QString &&a_value);
  void setValue (const QByteArray &a_name, const QString &a_value);
  void setValue (const QByteArray &a_name, QString &&a_value);

  static const QHash<QByteArray, FieldId> &fields();

  /* convert */

  static void fromJson (const QJsonObject &a_obj, DapNodeOrderInfo &a_dest);
  void fromJson (const QJsonObject &a_obj);
  QJsonObject toJsonObject() const;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  operator QJsonObject() const { return toJsonObject(); }
  DapNodeOrderInfo &operator= (const DapNodeOrderInfo &a_src);
  DapNodeOrderInfo &operator= (DapNodeOrderInfo &&a_src);
  /// @}
};

/*-----------------------------------------*/

QDebug operator<< (QDebug debug, const DapNodeOrderInfo &data);
QDataStream &operator<< (QDataStream &a_outStream, const DapNodeOrderInfo &a_serialKeyData);
QDataStream &operator>> (QDataStream &a_inStream, DapNodeOrderInfo &a_serialKeyData);

/*-----------------------------------------*/

typedef QList<DapNodeOrderInfo> DapNodeOrderInfoList;
typedef QHash<QString, DapNodeOrderInfo> DapNodeOrderInfoMap;

/*-----------------------------------------*/
#endif // DAPNODEORDERINFO_H
