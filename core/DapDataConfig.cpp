/* INCLUDES */

#include "DapDataConfig.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapDataConfig::DapDataConfig (const QString &a_filename)
  : QObject{}
  , _filename (a_filename)
  , m_msgCounter (1)
{
}

DapDataConfig::~DapDataConfig()
{

}

/********************************************
 * METHODS
 *******************************************/

int DapDataConfig::msgCounter() const
{
  return m_msgCounter;
}

void DapDataConfig::setMsgCounter (const int a_id)
{
  if (m_msgCounter < 100)
    m_msgCounter  = a_id;
}

QVariant DapDataConfig::value (const QString &a_name) const
{
  return _data.value (a_name);
}

bool DapDataConfig::setValue (const QString &a_name, const QVariant &a_value, const int a_msgId)
{
  if (!_isUniqueMessageId (a_msgId))
    return false;

  _data[a_name] = a_value;
  _changed << a_name;
  emit sigValueUpdated (a_name, a_value, a_msgId == 0 ? m_msgCounter++ : a_msgId);
  return true;
}

bool DapDataConfig::setValue (const QString &a_name, QVariant &&a_value, const int a_msgId)
{
  if (!_isUniqueMessageId (a_msgId))
    return false;

  _data[a_name] = a_value;
  _changed << a_name;
  emit sigValueUpdated (a_name, std::move (a_value), a_msgId == 0 ? m_msgCounter++ : a_msgId);
  return true;
}

bool DapDataConfig::remove (const QString &a_name, const int a_msgId)
{
  if (!_isUniqueMessageId (a_msgId))
    return false;

  _data.remove (a_name);
  _removed << a_name;
  emit sigValueRemoved (a_name, a_msgId == 0 ? m_msgCounter++ : a_msgId);
  return true;
}

QJsonObject DapDataConfig::toJson() const
{
  QJsonObject jobj;

  for (auto i = _data.constBegin(), e = _data.constEnd(); i != e; i++)
  {
    auto value  = i.value();
    if (value.type() == QVariant::ByteArray)
      jobj.insert (i.key(), QString ("base64,") + value.toByteArray().toBase64().constData());
    else
      jobj.insert (i.key(), value.toJsonValue());
  }

  return jobj;
}

void DapDataConfig::fromJson (const QJsonObject &a_json)
{
  static const char *base64sign = "base64,";

  for (auto i = a_json.constBegin(), e = a_json.constEnd(); i != e; i++)
  {
    auto value  = i.value();

    /* check if string */
    if (value.type() == QJsonValue::String)
    {
      auto stringValue = value.toString();

      /* check if base 64 */
      if (stringValue.startsWith (base64sign))
      {
        /* parse and store bytearray */
        auto base64src  = stringValue.mid (strlen (base64sign)).toUtf8();
        auto base64data = QByteArray::fromBase64 (base64src);
        _data[i.key()] = std::move (base64data);
      }

      /* store string */
      else
        _data[i.key()] = stringValue;
    }

    /* store value */
    else
      _data[i.key()] = value;
  }
}

/*-----------------------------------------*/

bool DapDataConfig::_isUniqueMessageId (int a_msgId)
{
  /* if no message id, set from msgCounter */
  if (a_msgId == 0)
    a_msgId = m_msgCounter; // return true;

  /* clear old */
  {
    int old = _receivedMessagesIds.size() - 256;
    if (old > 0)
    {
      if (old == 1)
        _receivedMessagesIds.erase (_receivedMessagesIds.begin());
      else
        _receivedMessagesIds.erase (_receivedMessagesIds.begin(), _receivedMessagesIds.begin() + old);
    }
  };

  /* not unique if found */
  if (_receivedMessagesIds.contains (a_msgId))
    return false;

  /* store new unique id */
  _receivedMessagesIds << a_msgId;
  return true;
}

/*-----------------------------------------*/
