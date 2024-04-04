/* INCLUDES */
#include "DapSerialKeyHistory.h"
#include "DapDataLocal.h"
#include "DapSerialKeyData.h"

/* DEFS */

#define DATA (*_d)

/*-----------------------------------------*/
/* serial key wrapper */

class SerialKey
{
  /* VARS */
  QString m_value;

  /* CONSTRUCT/DESTRUCT */
public:
  SerialKey();
  SerialKey (const SerialKey &a_src);
  SerialKey (SerialKey &&a_src);
  SerialKey (const QString &a_src);
  SerialKey (QString &&a_src);
  ~SerialKey();

  /* METHODS */
  QString value() const;
  operator QString() const { return value(); }

  /* OPERATORS */
  bool operator== (const SerialKey &a_value) const;
  SerialKey &operator= (const QString &a_value);
  SerialKey &operator= (QString &&a_value);
  SerialKey &operator= (const SerialKey &a_value);
  SerialKey &operator= (SerialKey &&a_value);
};

/*-----------------------------------------*/
/* data container */

class DapSerialKeyHistory::DapSerialKeyHistoryData
{
  /* VARS */
  QList<SerialKey> m_list;

  /* CONSTRUCT/DESTRUCT */
public:
  DapSerialKeyHistoryData();

  /* METHODS */
  void importList (const QStringList &a_list);
  QStringList exportList() const;

  void append (const QString &a_value);
  void append (QString &&a_value);
  void prepend (const QString &a_value);
  void prepend (QString &&a_value);
  QStringList list() const;
  const QString &value (int a_index) const;
  int size() const;
  bool contains (const QString &a_value) const;

  /* OPERATORS */
  operator QStringList() const;
  DapSerialKeyHistoryData &operator<< (const QString &a_value);
  DapSerialKeyHistoryData &operator<< (QString &&a_value);
  QString operator[] (int a_index)                { return value (a_index); }
  const QString &operator[] (int a_index) const   { return value (a_index); }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapSerialKeyHistory::DapSerialKeyHistory (QObject *parent)
  : QObject (parent)
  , _d (new DapSerialKeyHistoryData)
{
  //qRegisterMetaType<SerialKey> ("SerialKey");
}

DapSerialKeyHistory::~DapSerialKeyHistory()
{
  delete _d;
}

/********************************************
 * METHODS
 *******************************************/

const QStringList &DapSerialKeyHistory::list() const
{
  static QStringList v;
  v = DATA;
  return v;//m_list;
}

int DapSerialKeyHistory::size() const
{
  return DATA.size();//m_list.size();
}

QString DapSerialKeyHistory::value(int a_index) const
{
  return DATA[a_index];//m_list[a_index];
}

void DapSerialKeyHistory::load()
{
  QStringList list;
  DapDataLocal::instance()->loadFromSettings (TEXT_SERIAL_KEY_HISTORY, list);
  DATA.importList (list);
}

void DapSerialKeyHistory::save()
{
  QStringList list = DATA.exportList();
  DapDataLocal::instance()->saveToSettings (TEXT_SERIAL_KEY_HISTORY, list);
}

/********************************************
 * SLOTS
 *******************************************/

void DapSerialKeyHistory::slotNewKey()
{
  /* vars */
  auto key  = DapDataLocal::instance()->serialKeyData()->serialKey();

  /* check if already exists */
  if (DATA.contains (key))
    return;

  /* store and save */
  DATA.prepend (key);
  save();

  /* notify */
  emit sigNewKey();
}

/*-----------------------------------------*/

// 0000000000111111111
// 0123456789012345678
// ----+----+----+----
// 0000-0000-0000-0000
// 0x4, 0x9, 0xE


QString &&formatSerialKey (QString &&a_value)
{
  if (!a_value.contains('-'))
    {
      if (a_value.size() != 0x10)
        qWarning() << "DapSerialKeyHistory"
                   << __func__
                   << "internal issue! provided key is not formated and has invalid length:"
                   << a_value.size();

      a_value.insert (0xC, '-');
      a_value.insert (0x8, '-');
      a_value.insert (0x4, '-');
    }

  return std::move (a_value);
}

//QString formatSerialKey (QString a_value)
//{
//  return formatSerialKey (std::move (a_value));
//}

/*-----------------------------------------*/

/* CONSTRUCT/DESTRUCT */

SerialKey::SerialKey()
  : m_value()
{

}

SerialKey::SerialKey (const SerialKey &a_src)
{
  operator =(a_src);
}

SerialKey::SerialKey (SerialKey &&a_src)
{
  operator =(a_src);
}

SerialKey::SerialKey (const QString &a_src)
{
  operator =(a_src);
}

SerialKey::SerialKey (QString &&a_src)
{
  operator =(a_src);
}

SerialKey::~SerialKey()
{

}

/* METHODS */

QString SerialKey::value() const
{
  return m_value;
}

/* OPERATORS */

bool SerialKey::operator== (const SerialKey &a_value) const
{
  return m_value == a_value.m_value;
}

SerialKey &SerialKey::operator= (const QString &a_value)
{
  m_value = std::move (formatSerialKey (QString (a_value)));
  return *this;
}

SerialKey &SerialKey::operator= (QString &&a_value)
{
  m_value = std::move (formatSerialKey (std::move (a_value)));
  return *this;
}

SerialKey &SerialKey::operator= (const SerialKey &a_value)
{
  if (this != &a_value)
    m_value = std::move (formatSerialKey (QString (a_value)));

  return *this;
}

SerialKey &SerialKey::operator= (SerialKey &&a_value)
{
  if (this != &a_value)
    m_value = std::move (formatSerialKey (std::move (a_value.m_value)));

  return *this;
}

/*-----------------------------------------*/

/* CONSTRUCT/DESTRUCT */

DapSerialKeyHistory::DapSerialKeyHistoryData::DapSerialKeyHistoryData()
{

}

/* METHODS */

void DapSerialKeyHistory::DapSerialKeyHistoryData::importList (const QStringList &a_list)
{
  m_list.clear();

  for (const auto &key : a_list)
  {
    auto keyResult  = QString (key).remove ('-').remove (' ');
    if (!keyResult.isEmpty())
      append (std::move (keyResult));
  }
}

QStringList DapSerialKeyHistory::DapSerialKeyHistoryData::exportList() const
{
  return list();
}

void DapSerialKeyHistory::DapSerialKeyHistoryData::append (const QString &a_value)
{
  if (a_value.isEmpty())
    return;
  if (!contains (a_value))
    m_list.append (a_value);
}

void DapSerialKeyHistory::DapSerialKeyHistoryData::append (QString &&a_value)
{
  if (a_value.isEmpty())
    return;
  if (!contains (a_value))
    m_list.append (std::move (a_value));
}

void DapSerialKeyHistory::DapSerialKeyHistoryData::prepend (const QString &a_value)
{
  if (a_value.isEmpty())
    return;
  if (!contains (a_value))
    m_list.prepend (a_value);
}

void DapSerialKeyHistory::DapSerialKeyHistoryData::prepend (QString &&a_value)
{
  if (a_value.isEmpty())
    return;
  if (!contains (a_value))
    m_list.prepend (std::move (a_value));
}

QStringList DapSerialKeyHistory::DapSerialKeyHistoryData::list() const
{
  QStringList result;

  for (const auto &key : m_list)
    result << key;

  return result;
}

const QString &DapSerialKeyHistory::DapSerialKeyHistoryData::value (int a_index) const
{
  static QString v;
  v = m_list.at (a_index);
  return v;
}

int DapSerialKeyHistory::DapSerialKeyHistoryData::size() const
{
  return m_list.size();
}

bool DapSerialKeyHistory::DapSerialKeyHistoryData::contains (const QString &a_value) const
{
  SerialKey key (a_value);
  return m_list.contains (key);
}

/* OPERATORS */

DapSerialKeyHistory::DapSerialKeyHistoryData::operator QStringList() const
{
  return list();
}

DapSerialKeyHistory::DapSerialKeyHistoryData &DapSerialKeyHistory::DapSerialKeyHistoryData::operator<< (const QString &a_value)
{
  append (a_value);
  return *this;
}

DapSerialKeyHistory::DapSerialKeyHistoryData &DapSerialKeyHistory::DapSerialKeyHistoryData::operator<< (QString &&a_value)
{
  append (std::move (a_value));
  return *this;
}

/*-----------------------------------------*/
