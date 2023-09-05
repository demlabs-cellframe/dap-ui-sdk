/* INCLUDES */
#include "DapNodeOrderHistory.h"
#include "DapDataLocal.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

/* DEFS */
enum class FieldId : quint8
{
  INVALID   = int (DapNodeOrderInfo::FieldId::INVALID),

  wallet    = int (DapNodeOrderInfo::FieldId::version) + 1,
  network,
  token,
  value,
  unit,
  isSigned,
};

#define FIELD(a_name) { #a_name, FieldId::a_name }
#define INSERT_FIELD(a_name) insert ( int (FieldId::a_name), #a_name )

/* VARS */

// combined DapNodeOrderHistory::FieldId and DapNodeOrderInfo::FieldId roles map
static QHash<int, QByteArray> s_roles;

static const QHash<QByteArray, FieldId> s_fieldMap =
{
  FIELD (wallet),
  FIELD (network),
  FIELD (token),
  FIELD (value),
  FIELD (unit),
  FIELD (isSigned),
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapNodeOrderHistory::DapNodeOrderHistory (QObject *parent)
  : QAbstractListModel (parent)
{
  /* restore data */
  load();

  /* fill roles */
  if (s_roles.isEmpty())
  {
    /* inerhit */
    auto fields = DapNodeOrderInfo::fields();
    for (auto i = fields.cbegin(), e = fields.cend(); i != e; i++)
      s_roles.insert (int (i.value()), i.key());

    /* append */
    s_roles.INSERT_FIELD (wallet);
    s_roles.INSERT_FIELD (network);
    s_roles.INSERT_FIELD (token);
    s_roles.INSERT_FIELD (value);
    s_roles.INSERT_FIELD (unit);
    s_roles.INSERT_FIELD (isSigned);
  }
}

/********************************************
 * METHODS
 *******************************************/

DapNodeOrderHistory *DapNodeOrderHistory::instance()
{
  static DapNodeOrderHistory *i = nullptr;

  if (i == nullptr)
  {
    i = new DapNodeOrderHistory;
    connect (i, &QObject::destroyed,
             [=] { i = nullptr; });
  }

  return i;
}

const DapNodeOrderHistory::Order &DapNodeOrderHistory::at (int a_index) const
{
  return _list.at (a_index);
}

int DapNodeOrderHistory::size() const
{
  return _list.size();
}

void DapNodeOrderHistory::append (const Order &a_value)
{
  beginInsertRows (QModelIndex(), size(), size());
  _list.append (a_value);
  endInsertRows();
}

void DapNodeOrderHistory::append (DapNodeOrderHistory::Order &&a_value)
{
  beginInsertRows (QModelIndex(), size(), size());
  _list.append (std::move (a_value));
  endInsertRows();
}

void DapNodeOrderHistory::prepend (const DapNodeOrderHistory::Order &a_value)
{
  beginInsertRows (QModelIndex(), 0, 0);
  _list.prepend (a_value);
  endInsertRows();
}

void DapNodeOrderHistory::prepend (DapNodeOrderHistory::Order &&a_value)
{
  beginInsertRows (QModelIndex(), 0, 0);
  _list.prepend (std::move (a_value));
  endInsertRows();
}

void DapNodeOrderHistory::insert (int a_index, const DapNodeOrderHistory::Order &a_value)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  _list.insert (a_index, a_value);
  endInsertRows();
}

void DapNodeOrderHistory::insert (int a_index, DapNodeOrderHistory::Order &&a_value)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  _list.insert (a_index, std::move (a_value));
  endInsertRows();
}

void DapNodeOrderHistory::insert (Iterator a_index, const DapNodeOrderHistory::Order &a_value)
{
  int index = std::distance (_list.begin(), a_index);
  beginInsertRows (QModelIndex(), index, index);
  _list.insert (a_index, a_value);
  endInsertRows();
}

void DapNodeOrderHistory::insert (Iterator a_index, DapNodeOrderHistory::Order &&a_value)
{
  int index = std::distance (_list.begin(), a_index);
  beginInsertRows (QModelIndex(), index, index);
  _list.insert (a_index, std::move (a_value));
  endInsertRows();
}

DapNodeOrderHistory::Iterator DapNodeOrderHistory::begin()
{
  return _list.begin();
}

DapNodeOrderHistory::ConstIterator DapNodeOrderHistory::begin() const
{
  return _list.begin();
}

DapNodeOrderHistory::ConstIterator DapNodeOrderHistory::cbegin() const
{
  return _list.begin();
}

DapNodeOrderHistory::Iterator DapNodeOrderHistory::end()
{
  return _list.end();
}

DapNodeOrderHistory::ConstIterator DapNodeOrderHistory::end() const
{
  return _list.end();
}

DapNodeOrderHistory::ConstIterator DapNodeOrderHistory::cend() const
{
  return _list.end();
}

int DapNodeOrderHistory::currentIndex() const
{
  return m_currentIndex;
}

void DapNodeOrderHistory::setCurrentIndex (int a_value)
{
  if (m_currentIndex == a_value)
    return;

  m_currentIndex  = a_value;
  emit currentIndexChanged();
}

const DapNodeOrderHistory::Order &DapNodeOrderHistory::current() const
{
  static const DapNodeOrderHistory::Order dummy{};
  if (m_currentIndex < 0
      || m_currentIndex >= size())
    return dummy;
  return _list.at (m_currentIndex);
}

int DapNodeOrderHistory::indexOf (const QString &a_orderHash) const
{
  int index = 0;
  for (auto i = _list.cbegin(), e = _list.cend(); i != e; i++, index++)
    if (i->info.hash() == a_orderHash)
      return index;
  return -1;
}

void DapNodeOrderHistory::load()
{
  beginResetModel();

  /* vars */
  QByteArray source;
  QJsonArray jarr;

  /* get data */
  DapDataLocal::instance()->loadFromSettings (NODE_ORDER_HISTORY, source);
  jarr  = QJsonDocument::fromJson (source).array();

  /* prepare parsing */
  _list.clear();

  /* parse json array */
  for (const auto &jitem : qAsConst (jarr))
  {
    DapNodeOrderHistory::Order item;
    item.fromJson (jitem.toObject());
    _list.append (std::move (item));
  }

  endResetModel();
}

void DapNodeOrderHistory::save() const
{
  QJsonArray jarr;

  /* collect json items */
  for (const auto &item : _list)
    jarr.append (QJsonObject (item));

  /* store result json array */
  QByteArray result = QJsonDocument (jarr).toJson (QJsonDocument::JsonFormat::Compact);
  DapDataLocal::instance()->saveToSettings (NODE_ORDER_HISTORY, result);
}

void DapNodeOrderHistory::itemUpdated (int a_index)
{
  emit dataChanged (index (a_index), index (a_index));
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapNodeOrderHistory::rowCount (const QModelIndex &) const
{
  return size();
}

QVariant DapNodeOrderHistory::data (const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  auto &item  = at (index.row());

  switch (FieldId (role))
  {
  case FieldId::INVALID:  break;
  case FieldId::wallet:   return item.wallet;
  case FieldId::network:  return item.network;
  case FieldId::token:    return item.token;
  case FieldId::value:    return item.value;
  case FieldId::unit:     return item.unit;
  case FieldId::isSigned: return item.isSigned;
  }

  return item.info.value (s_roles.value (role));
}

QHash<int, QByteArray> DapNodeOrderHistory::roleNames() const
{
  return s_roles;
}

/********************************************
 * OPERATORS
 *******************************************/

DapNodeOrderHistory::Order &DapNodeOrderHistory::operator[] (int a_index)
{
  return _list[a_index];
}

/*-----------------------------------------*/

void DapNodeOrderHistory::Order::fromJson (const QJsonObject &a_obj)
{
  for (auto i = a_obj.constBegin(), e = a_obj.constEnd(); i != e; i++)
  {
    /* if order info */
    if (i.key() == "info")
    {
      info.fromJson (i.value().toObject());
      continue;
    }

    /* get field id and value */
    FieldId fid   = s_fieldMap.value (i.key().toUtf8(), FieldId::INVALID); // fast search
    QString val   = i.value().toString();

    /* store by id */
    switch (fid) // fast branch table by id
    {
    case FieldId::INVALID:  break;
    case FieldId::wallet:   wallet    = std::move (val); break;
    case FieldId::network:  network   = std::move (val); break;
    case FieldId::token:    token     = std::move (val); break;
    case FieldId::value:    value     = std::move (val); break;
    case FieldId::unit:     unit      = std::move (val); break;
    case FieldId::isSigned: isSigned  = i.value().toBool(); break;
    }
  }
}

QJsonObject DapNodeOrderHistory::Order::toJsonObject() const
{
  return QJsonObject {
    { "info",     info.toJsonObject() },
    { "wallet",   wallet },
    { "network",  network },
    { "token",    token },
    { "value",    value },
    { "unit",     unit },
    { "isSigned", isSigned },
  };
}

/*-----------------------------------------*/
