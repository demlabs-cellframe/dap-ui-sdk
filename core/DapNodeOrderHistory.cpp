/* INCLUDES */
#include "DapNodeOrderHistory.h"
#include "DapDataLocal.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

/* VARS */
static QHash<int, QByteArray> s_roles;

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
    auto fields = DapNodeOrderInfo::fields();
    for (auto i = fields.cbegin(), e = fields.cend(); i != e; i++)
      s_roles.insert (int (i.value()), i.key());
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

const DapNodeOrderInfo &DapNodeOrderHistory::at (int a_index) const
{
  return _list.at (a_index);
}

int DapNodeOrderHistory::size() const
{
  return _list.size();
}

void DapNodeOrderHistory::append (const DapNodeOrderInfo &a_value)
{
  beginInsertRows (QModelIndex(), size(), size());
  _list.append (a_value);
  endInsertRows();
}

void DapNodeOrderHistory::append (DapNodeOrderInfo &&a_value)
{
  beginInsertRows (QModelIndex(), size(), size());
  _list.append (std::move (a_value));
  endInsertRows();
}

void DapNodeOrderHistory::prepend (const DapNodeOrderInfo &a_value)
{
  beginInsertRows (QModelIndex(), 0, 0);
  _list.prepend (a_value);
  endInsertRows();
}

void DapNodeOrderHistory::prepend (DapNodeOrderInfo &&a_value)
{
  beginInsertRows (QModelIndex(), 0, 0);
  _list.prepend (std::move (a_value));
  endInsertRows();
}

void DapNodeOrderHistory::insert (int a_index, const DapNodeOrderInfo &a_value)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  _list.insert (a_index, a_value);
  endInsertRows();
}

void DapNodeOrderHistory::insert (int a_index, DapNodeOrderInfo &&a_value)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  _list.insert (a_index, std::move (a_value));
  endInsertRows();
}

void DapNodeOrderHistory::insert (Iterator a_index, const DapNodeOrderInfo &a_value)
{
  int index = std::distance (_list.begin(), a_index);
  beginInsertRows (QModelIndex(), index, index);
  _list.insert (a_index, a_value);
  endInsertRows();
}

void DapNodeOrderHistory::insert (Iterator a_index, DapNodeOrderInfo &&a_value)
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

const DapNodeOrderInfo &DapNodeOrderHistory::current() const
{
  static const DapNodeOrderInfo dummy;
  if (m_currentIndex < 0
      || m_currentIndex >= size())
    return dummy;
  return _list.at (m_currentIndex);
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
    DapNodeOrderInfo item;
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

  return at (index.row()).value (s_roles.value (role));
}

QHash<int, QByteArray> DapNodeOrderHistory::roleNames() const
{
  return s_roles;
}

/********************************************
 * OPERATORS
 *******************************************/

DapNodeOrderInfo &DapNodeOrderHistory::operator[] (int a_index)
{
  return _list[a_index];
}

/*-----------------------------------------*/
