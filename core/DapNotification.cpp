/* INCLUDES */

#include "DapNotification.h"

/* DEFS */

#define ROLEFIELD(a_name) { int (DapNotification::FieldId::a_name), #a_name }
#define FIELDID(a_name)   { #a_name, DapNotification::FieldId::a_name }

/* VARS */

static const QHash<int, QByteArray> s_roles =
{
  ROLEFIELD (created),
  ROLEFIELD (message),
  ROLEFIELD (type),
};

static const QHash<QString, DapNotification::FieldId> s_fieldIdMap =
{
  FIELDID (created),
  FIELDID (message),
  FIELDID (type),
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapNotification::DapNotification (QObject *parent)
  : QObject (parent)
{

}

DapNotification::DapNotification (const DapNotification &a_source)
  : QObject()
{
  operator = (a_source);
}

DapNotification::DapNotification (DapNotification &&a_source) noexcept
{
  operator = (std::move (a_source));
}

DapNotification::DapNotification (const QDateTime &a_created, const QString &a_message, Type a_type)
  : m_created (a_created)
  , m_message (a_message)
  , m_type (a_type)
{

}

DapNotification::~DapNotification()
{

}

/********************************************
 * METHODS
 *******************************************/

const QDateTime &DapNotification::created() const
{
  return m_created;
}

QString DapNotification::createdString() const
{
  return m_created.toString ("hh:mm:ss dd.MM.yyyy");
}

void DapNotification::setCreated (const QDateTime &a_value)
{
  if (m_created == a_value)
    return;

  m_created = a_value;
}

void DapNotification::setCreated (const QString &a_value)
{
  auto value  = QDateTime::fromString (a_value, "hh:mm:ss dd.MM.yyyy");
  setCreated (value);
}

const QString &DapNotification::message() const
{
  return m_message;
}

void DapNotification::setMessage (const QString &a_value)
{
  if (m_message == a_value)
    return;

  m_message = a_value;
}

DapNotification::Type DapNotification::type() const
{
  return m_type;
}

void DapNotification::setType (Type a_value)
{
  if (m_type == a_value)
    return;

  m_type = a_value;
}

QVariant DapNotification::value (const char *a_name) const
{
  auto fid  = fieldId (a_name);

  if (fid == FieldId::invalid)
    return QVariant();

  return value (fid);
}

QVariant DapNotification::value (FieldId a_fid) const
{
  switch (a_fid)
    {
    case FieldId::invalid:  break;
    case FieldId::created:  return createdString();
    case FieldId::message:  return message();
    case FieldId::type:     return int (type());
    }

  return QVariant();
}

void DapNotification::setValue (const char *a_name, const QVariant &a_value)
{
  auto fid  = fieldId (a_name);

  if (fid == FieldId::invalid)
    return;

  setValue (fid, a_value);
}

void DapNotification::setValue (FieldId a_fid, const QVariant &a_value)
{
  switch (a_fid)
    {
    case FieldId::invalid:  break;
    case FieldId::created:
      a_value.type() == QVariant::Type::String
      ? setCreated (a_value.toString())
      : setCreated (a_value.toDateTime());
      break;
    case FieldId::message:  setMessage (a_value.toString()); break;
    case FieldId::type:     setType (Type (a_value.toInt())); break;
  }
}

DapNotification::FieldId DapNotification::fieldId (const char *a_name)
{
  return s_fieldIdMap.value (a_name, FieldId::invalid);
}

const QHash<int, QByteArray> &DapNotification::fields()
{
  return s_roles;
}

DapNotification DapNotification::fromJson (const QJsonObject &a_source)
{
  /* result */
  DapNotification result;

  /* parse map */
  typedef void (*ReadField) (const QJsonValue & a_value, DapNotification & a_dest);
  static const QHash<QString, ReadField> parseMap =
  {
    { "created",  [] (const QJsonValue & a_value, DapNotification & a_dest) { a_dest.setCreated (a_value.toString()); }},
    { "message",  [] (const QJsonValue & a_value, DapNotification & a_dest) { a_dest.setMessage (a_value.toString()); }},
    { "type",     [] (const QJsonValue & a_value, DapNotification & a_dest) { a_dest.setType (Type (a_value.toInt())); }},
  };

  /*  parse every field */
  for (auto i = a_source.begin(), e = a_source.end(); i != e; i++)
    {
      auto readField  = parseMap.value (i.key(), nullptr);
      if (readField)
        readField (i.value(), result);
    }

  return result;
}

QJsonObject DapNotification::toJson() const
{
  return QJsonObject
  {
    { "created", createdString() },
    { "message", message() },
    { "type",    int (type()) },
  };
}

/********************************************
 * OPERATORS
 *******************************************/

DapNotification &DapNotification::operator= (const DapNotification &a_source)
{
  if (this != &a_source)
    {
      m_created = a_source.m_created;
      m_message = a_source.m_message;
      m_type = a_source.m_type;
    }

  return *this;
}

DapNotification &DapNotification::operator= (DapNotification &&a_source) noexcept
{
  if (this != &a_source)
    {
      m_created = std::move (a_source.m_created);
      m_message = std::move (a_source.m_message);
      m_type = std::move (a_source.m_type);
    }

  return *this;
}

/*-----------------------------------------*/
