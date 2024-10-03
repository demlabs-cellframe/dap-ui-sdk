/* INCLUDES */

#include "DapDataConfig.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapDataConfig::DapDataConfig (const QString &a_filename)
  : QObject{}
  , _filename (a_filename)
  , _msgCounter (1)
{
}

/********************************************
 * METHODS
 *******************************************/

void DapDataConfig::setMsgCounter (const int a_id)
{
  _msgCounter  = a_id;
}

QVariant DapDataConfig::value (const QString &a_name) const
{
  return _data.value (a_name);
}

void DapDataConfig::setValue (const QString &a_name, const QVariant &a_value, const int a_msgId)
{
  if (!_isUniqueMessageId (a_msgId))
    return;

  _data[a_name] = a_value;
  _changed << a_name;
  emit sigValueUpdated (a_name, a_value, a_msgId == 0 ? _msgCounter++ : a_msgId);
}

void DapDataConfig::setValue (const QString &a_name, QVariant &&a_value, const int a_msgId)
{
  if (!_isUniqueMessageId (a_msgId))
    return;

  _data[a_name] = a_value;
  _changed << a_name;
  emit sigValueUpdated (a_name, std::move (a_value), a_msgId == 0 ? _msgCounter++ : a_msgId);
}

void DapDataConfig::remove (const QString &a_name, const int a_msgId)
{
  if (!_isUniqueMessageId (a_msgId))
    return;

  _data.remove (a_name);
  _removed << a_name;
  emit sigValueRemoved (a_name, a_msgId == 0 ? _msgCounter++ : a_msgId);
}

QJsonObject DapDataConfig::toJson() const
{
  QJsonObject jobj;

  for (auto i = _data.constBegin(), e = _data.constEnd(); i != e; i++)
    jobj.insert (i.key(), i.value().toJsonValue());

  return jobj;
}

void DapDataConfig::fromJson (const QJsonObject &a_json)
{
  for (auto i = a_json.constBegin(), e = a_json.constEnd(); i != e; i++)
    setValue (i.key(), i.value().toVariant());
}

/*-----------------------------------------*/

bool DapDataConfig::_isUniqueMessageId (int a_msgId)
{
  /* if no message id, return as unique */
  if (a_msgId == 0)
    return true;

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
