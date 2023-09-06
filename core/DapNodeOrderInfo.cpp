/* INCLUDES */
#include "DapNodeOrderInfo.h"

#include <QJsonDocument>

/* DEFS */
typedef DapNodeOrderInfo::FieldId FieldId;
#define FIELD(a_name) { #a_name, FieldId::a_name }

/* VARS */

static const QHash<QByteArray, FieldId> s_fieldMap =
{
  FIELD (direction),
  FIELD (ext),
  FIELD (hash),
  FIELD (nodeAddress),
  FIELD (nodeLocation),
  FIELD (pkey),
  FIELD (price),
  FIELD (priceUnit),
  FIELD (srvUid),
  FIELD (txCondHash),
  FIELD (units),
  FIELD (version),

  /* minor bug fix */
  { "node_addr",      FieldId::nodeAddress },
  { "node_location",  FieldId::nodeLocation },
  { "price_unit",     FieldId::priceUnit },
  { "srv_uid",        FieldId::srvUid },
  { "tx_cond_hash",   FieldId::txCondHash },
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapNodeOrderInfo::DapNodeOrderInfo()
{

}

DapNodeOrderInfo::DapNodeOrderInfo (const DapNodeOrderInfo &a_src)
{
  operator= (a_src);
}

DapNodeOrderInfo::DapNodeOrderInfo (DapNodeOrderInfo &&a_src)
{
  operator= (std::move (a_src));
}

DapNodeOrderInfo::~DapNodeOrderInfo()
{

}

/********************************************
 * METHODS
 *******************************************/

const QString &DapNodeOrderInfo::direction() const
{
  return m_direction;
}

void DapNodeOrderInfo::setDirection (const QString &a_value)
{
  m_direction = a_value;
}

const QString &DapNodeOrderInfo::ext() const
{
  return m_ext;
}

void DapNodeOrderInfo::setExt (const QString &a_value)
{
  m_ext = a_value;
}

const QString &DapNodeOrderInfo::hash() const
{
  return m_hash;
}

void DapNodeOrderInfo::setHash (const QString &a_value)
{
  m_hash = a_value;
}

const QString &DapNodeOrderInfo::nodeAddress() const
{
  return m_nodeAddress;
}

void DapNodeOrderInfo::setNodeAddress (const QString &a_value)
{
  m_nodeAddress = a_value;
}

const QString &DapNodeOrderInfo::nodeLocation() const
{
  return m_nodeLocation;
}

void DapNodeOrderInfo::setNodeLocation (const QString &a_value)
{
  m_nodeLocation = a_value;
}

const QString &DapNodeOrderInfo::pkey() const
{
  return m_pkey;
}

void DapNodeOrderInfo::setPkey (const QString &a_value)
{
  m_pkey = a_value;
}

const QString &DapNodeOrderInfo::price() const
{
  return m_price;
}

void DapNodeOrderInfo::setPrice (const QString &a_value)
{
  m_price = a_value;
}

const QString &DapNodeOrderInfo::priceUnit() const
{
  return m_priceUnit;
}

void DapNodeOrderInfo::setPriceUnit (const QString &a_value)
{
  m_priceUnit = a_value;
}

const QString &DapNodeOrderInfo::srvUid() const
{
  return m_srvUid;
}

void DapNodeOrderInfo::setSrvUid (const QString &a_value)
{
  m_srvUid = a_value;
}

const QString &DapNodeOrderInfo::txCondHash() const
{
  return m_txCondHash;
}

void DapNodeOrderInfo::setTxCondHash (const QString &a_value)
{
  m_txCondHash = a_value;
}

const QString &DapNodeOrderInfo::units() const
{
  return m_units;
}

void DapNodeOrderInfo::setUnits (const QString &a_value)
{
  m_units = a_value;
}

const QString &DapNodeOrderInfo::version() const
{
  return m_version;
}

void DapNodeOrderInfo::setVersion (const QString &a_value)
{
  m_version = a_value;
}

const QString &DapNodeOrderInfo::value (FieldId a_fid) const
{
  static const QString dummy;

  /* get by id */
  switch (a_fid) // fast branch table by id
    {
      case FieldId::INVALID:      break;
      case FieldId::direction:    return m_direction;
      case FieldId::ext:          return m_ext;
      case FieldId::hash:         return m_hash;
      case FieldId::nodeAddress:  return m_nodeAddress;
      case FieldId::nodeLocation: return m_nodeLocation;
      case FieldId::pkey:         return m_pkey;
      case FieldId::price:        return m_price;
      case FieldId::priceUnit:    return m_priceUnit;
      case FieldId::srvUid:       return m_srvUid;
      case FieldId::txCondHash:   return m_txCondHash;
      case FieldId::units:        return m_units;
      case FieldId::version:      return m_version;
    }

  return dummy;
}

const QString &DapNodeOrderInfo::value (const QByteArray &a_name) const
{
  FieldId fid = s_fieldMap.value (a_name, FieldId::INVALID); // fast search
  return value (fid);
}

void DapNodeOrderInfo::setValue (FieldId a_fid, const QString &a_value)
{
  /* set by id */
  switch (a_fid) // fast branch table by id
    {
      case FieldId::INVALID:      break;
      case FieldId::direction:    setDirection (a_value); break;
      case FieldId::ext:          setExt (a_value); break;
      case FieldId::hash:         setHash (a_value); break;
      case FieldId::nodeAddress:  setNodeAddress (a_value); break;
      case FieldId::nodeLocation: setNodeLocation (a_value); break;
      case FieldId::pkey:         setPkey (a_value); break;
      case FieldId::price:        setPrice (a_value); break;
      case FieldId::priceUnit:    setPriceUnit (a_value); break;
      case FieldId::srvUid:       setSrvUid (a_value); break;
      case FieldId::txCondHash:   setTxCondHash (a_value); break;
      case FieldId::units:        setUnits (a_value); break;
      case FieldId::version:      setVersion (a_value); break;
    }
}

void DapNodeOrderInfo::setValue (FieldId a_fid, QString &&a_value)
{
  /* set by id */
  switch (a_fid) // fast branch table by id
    {
      case FieldId::INVALID:      break;
      case FieldId::direction:    m_direction    = std::move (a_value); break;
      case FieldId::ext:          m_ext          = std::move (a_value); break;
      case FieldId::hash:         m_hash         = std::move (a_value); break;
      case FieldId::nodeAddress:  m_nodeAddress  = std::move (a_value); break;
      case FieldId::nodeLocation: m_nodeLocation = std::move (a_value); break;
      case FieldId::pkey:         m_pkey         = std::move (a_value); break;
      case FieldId::price:        m_price        = std::move (a_value); break;
      case FieldId::priceUnit:    m_priceUnit    = std::move (a_value); break;
      case FieldId::srvUid:       m_srvUid       = std::move (a_value); break;
      case FieldId::txCondHash:   m_txCondHash   = std::move (a_value); break;
      case FieldId::units:        m_units        = std::move (a_value); break;
      case FieldId::version:      m_version      = std::move (a_value); break;
    }
}

void DapNodeOrderInfo::setValue (const QByteArray &a_name, const QString &a_value)
{
  FieldId fid = s_fieldMap.value (a_name, FieldId::INVALID); // fast search
  setValue (fid, a_value);
}

void DapNodeOrderInfo::setValue (const QByteArray &a_name, QString &&a_value)
{
  FieldId fid = s_fieldMap.value (a_name, FieldId::INVALID); // fast search
  setValue (fid, a_value);
}

const QHash<QByteArray, DapNodeOrderInfo::FieldId> &DapNodeOrderInfo::fields()
{
  return s_fieldMap;
}


void DapNodeOrderInfo::fromJson (const QJsonObject &a_obj, DapNodeOrderInfo &a_dest)
{
  for (auto i = a_obj.constBegin(), e = a_obj.constEnd(); i != e; i++)
    a_dest.setValue (i.key().toUtf8(), i.value().toString());
}

void DapNodeOrderInfo::fromJson (const QJsonObject &a_obj)
{
  for (auto i = a_obj.constBegin(), e = a_obj.constEnd(); i != e; i++)
    setValue (i.key().toUtf8(), i.value().toString());
}

QJsonObject DapNodeOrderInfo::toJsonObject() const
{
  return QJsonObject {
    { "direction",    m_direction },
    { "ext",          m_ext },
    { "hash",         m_hash },
    { "nodeAddress",  m_nodeAddress },
    { "nodeLocation", m_nodeLocation },
    { "pkey",         m_pkey },
    { "price",        m_price },
    { "priceUnit",    m_priceUnit },
    { "srvUid",       m_srvUid },
    { "txCondHash",   m_txCondHash },
    { "units",        m_units },
    { "version",      m_version },
  };
}

/********************************************
 * OPERATORS
 *******************************************/

DapNodeOrderInfo &DapNodeOrderInfo::operator= (const DapNodeOrderInfo &a_src)
{
  if (&a_src != this)
  {
    m_direction     = a_src.m_direction;
    m_ext           = a_src.m_ext;
    m_hash          = a_src.m_hash;
    m_nodeAddress   = a_src.m_nodeAddress;
    m_nodeLocation  = a_src.m_nodeLocation;
    m_pkey          = a_src.m_pkey;
    m_price         = a_src.m_price;
    m_priceUnit     = a_src.m_priceUnit;
    m_srvUid        = a_src.m_srvUid;
    m_txCondHash    = a_src.m_txCondHash;
    m_units         = a_src.m_units;
    m_version       = a_src.m_version;
  }

  return *this;
}

DapNodeOrderInfo &DapNodeOrderInfo::operator= (DapNodeOrderInfo &&a_src)
{
  if (&a_src != this)
  {
    m_direction     = std::move (a_src.m_direction);
    m_ext           = std::move (a_src.m_ext);
    m_hash          = std::move (a_src.m_hash);
    m_nodeAddress   = std::move (a_src.m_nodeAddress);
    m_nodeLocation  = std::move (a_src.m_nodeLocation);
    m_pkey          = std::move (a_src.m_pkey);
    m_price         = std::move (a_src.m_price);
    m_priceUnit     = std::move (a_src.m_priceUnit);
    m_srvUid        = std::move (a_src.m_srvUid);
    m_txCondHash    = std::move (a_src.m_txCondHash);
    m_units         = std::move (a_src.m_units);
    m_version       = std::move (a_src.m_version);
  }

  return *this;
}

/*-----------------------------------------*/

QDebug operator<< (QDebug debug, const DapNodeOrderInfo &data)
{
  debug.nospace() << "OrderInfo("
                  << "direction:" << data.direction()
                  << ", ext:" << data.ext()
                  << ", hash:" << data.hash()
                  << ", nodeAddress:" << data.nodeAddress()
                  << ", nodeLocation:" << data.nodeLocation()
                  << ", pkey:" << data.pkey()
                  << ", price:" << data.price()
                  << ", priceUnit:" << data.priceUnit()
                  << ", srvUid:" << data.srvUid()
                  << ", txCondHash:" << data.txCondHash()
                  << ", units:" << data.units()
                  << ", version:" << data.version() << ")";
  return debug.space();
}

QDataStream &operator<< (QDataStream &a_outStream, const DapNodeOrderInfo &a_value)
{
  /* write as compact json */
  a_outStream << QJsonDocument (a_value).toJson (QJsonDocument::JsonFormat::Compact);
  return a_outStream;
}

QDataStream &operator>> (QDataStream &a_inStream, DapNodeOrderInfo &a_dest)
{
  /* read */
  QByteArray json;
  a_inStream >> json;

  /* parse */
  a_dest.fromJson (QJsonDocument::fromJson (json).object());

  return a_inStream;
}

/*-----------------------------------------*/
