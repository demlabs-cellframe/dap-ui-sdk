/* INCLUDES */
#include "DapNodeTransactionHistory.h"
#include "DapDataLocal.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>

/* DEFS */
enum class FieldId : quint8
{
  INVALID   = int (DapNodeOrderInfo::FieldId::INVALID),

  ipAddress = int (DapNodeOrderInfo::FieldId::SIZE),
  network,
  wallet,
  token,
  tokenValue,
  unit,
  unitValue,
  priceValue,
  portions,

  totalValue,
  created,
  isSigned,

  SIZE,
};

#define FIELD(a_name) { #a_name, FieldId::a_name }
#define INSERT_FIELD(a_name) insert ( int (FieldId::a_name), #a_name )

/* VARS */

// combined DapNodeOrderHistory::FieldId and DapNodeOrderInfo::FieldId roles map
static QHash<int, QByteArray> s_roles;

static const QHash<QByteArray, FieldId> s_fieldMap =
{
  FIELD (ipAddress),
  FIELD (network),
  FIELD (wallet),
  FIELD (token),
  FIELD (tokenValue),
  FIELD (unit),
  FIELD (unitValue),
  FIELD (priceValue),
  FIELD (portions),

  FIELD (totalValue),
  FIELD (created),
  FIELD (isSigned),
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapNodeTransactionHistory::DapNodeTransactionHistory (QObject *parent)
  : QAbstractListModel (parent)
{
  /* fill roles */
  if (s_roles.isEmpty())
  {
    /* inerhit */
    auto fields = DapNodeOrderInfo::fields();
    for (auto i = fields.cbegin(), e = fields.cend(); i != e; i++)
      s_roles.insert (int (i.value()), i.key());

    /* append */
    s_roles.INSERT_FIELD (ipAddress);
    s_roles.INSERT_FIELD (network);
    s_roles.INSERT_FIELD (wallet);
    s_roles.INSERT_FIELD (token);
    s_roles.INSERT_FIELD (tokenValue);
    s_roles.INSERT_FIELD (unit);
    s_roles.INSERT_FIELD (unitValue);
    s_roles.INSERT_FIELD (priceValue);
    s_roles.INSERT_FIELD (portions);

    s_roles.INSERT_FIELD (totalValue);
    s_roles.INSERT_FIELD (created);
    s_roles.INSERT_FIELD (isSigned);

    //qDebug().nospace() << "DapNodeTransactionHistory roles:" << s_roles;
  }
}

/********************************************
 * METHODS
 *******************************************/

DapNodeTransactionHistory *DapNodeTransactionHistory::instance()
{
  static DapNodeTransactionHistory *i = nullptr;

  if (i == nullptr)
  {
    i = new DapNodeTransactionHistory;
    connect (i, &QObject::destroyed,
             [=] { i = nullptr; });
  }

  return i;
}

const DapNodeTransactionHistory::Transaction &DapNodeTransactionHistory::at (int a_index) const
{
  return _list.at (a_index);
}

int DapNodeTransactionHistory::size() const
{
  return _list.size();
}

void DapNodeTransactionHistory::append (const Transaction &a_value)
{
  int newIndex  = size();
  beginInsertRows (QModelIndex(), newIndex, newIndex);
  _list.append (a_value);
  endInsertRows();
  setCurrentIndex (newIndex);
  _delayedSave();
}

void DapNodeTransactionHistory::append (DapNodeTransactionHistory::Transaction &&a_value)
{
  int newIndex  = size();
  beginInsertRows (QModelIndex(), newIndex, newIndex);
  _list.append (std::move (a_value));
  endInsertRows();
  setCurrentIndex (newIndex);
  _delayedSave();
}

void DapNodeTransactionHistory::prepend (const DapNodeTransactionHistory::Transaction &a_value)
{
  beginInsertRows (QModelIndex(), 0, 0);
  _list.prepend (a_value);
  endInsertRows();
  setCurrentIndex (0);
  _delayedSave();
}

void DapNodeTransactionHistory::prepend (DapNodeTransactionHistory::Transaction &&a_value)
{
  beginInsertRows (QModelIndex(), 0, 0);
  _list.prepend (std::move (a_value));
  endInsertRows();
  setCurrentIndex (0);
  _delayedSave();
}

void DapNodeTransactionHistory::insert (int a_index, const DapNodeTransactionHistory::Transaction &a_value)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  _list.insert (a_index, a_value);
  endInsertRows();
  _delayedSave();
}

void DapNodeTransactionHistory::insert (int a_index, DapNodeTransactionHistory::Transaction &&a_value)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  _list.insert (a_index, std::move (a_value));
  endInsertRows();
  _delayedSave();
}

void DapNodeTransactionHistory::insert (Iterator a_index, const DapNodeTransactionHistory::Transaction &a_value)
{
  int index = std::distance (_list.begin(), a_index);
  beginInsertRows (QModelIndex(), index, index);
  _list.insert (a_index, a_value);
  endInsertRows();
  _delayedSave();
}

void DapNodeTransactionHistory::insert (Iterator a_index, DapNodeTransactionHistory::Transaction &&a_value)
{
  int index = std::distance (_list.begin(), a_index);
  beginInsertRows (QModelIndex(), index, index);
  _list.insert (a_index, std::move (a_value));
  endInsertRows();
  _delayedSave();
}

DapNodeTransactionHistory::Iterator DapNodeTransactionHistory::begin()
{
  return _list.begin();
}

DapNodeTransactionHistory::ConstIterator DapNodeTransactionHistory::begin() const
{
  return _list.begin();
}

DapNodeTransactionHistory::ConstIterator DapNodeTransactionHistory::cbegin() const
{
  return _list.begin();
}

DapNodeTransactionHistory::Iterator DapNodeTransactionHistory::end()
{
  return _list.end();
}

DapNodeTransactionHistory::ConstIterator DapNodeTransactionHistory::end() const
{
  return _list.end();
}

DapNodeTransactionHistory::ConstIterator DapNodeTransactionHistory::cend() const
{
  return _list.end();
}

int DapNodeTransactionHistory::currentIndex() const
{
  return m_currentIndex;
}

void DapNodeTransactionHistory::setCurrentIndex (int a_value)
{
  if (m_currentIndex == a_value)
    return;

  m_currentIndex  = a_value;
  emit currentIndexChanged();
}

const DapNodeTransactionHistory::Transaction &DapNodeTransactionHistory::current() const
{
  static const DapNodeTransactionHistory::Transaction dummy{};
  if (m_currentIndex < 0
      || m_currentIndex >= size())
    return dummy;
  return _list.at (m_currentIndex);
}

int DapNodeTransactionHistory::indexOf (const QString &a_orderHash) const
{
  int index = 0;
  for (auto i = _list.cbegin(), e = _list.cend(); i != e; i++, index++)
    if (i->info.hash() == a_orderHash)
      return index;
  return -1;
}

void DapNodeTransactionHistory::load()
{
  /* get data */
  if (m_pCmdDataLocal != nullptr)
  {
    m_isFromSend = false;
    m_pCmdDataLocal->requestValue(DapDataLocal::NODE_ORDER_HISTORY);
  }
}

void DapNodeTransactionHistory::save() const
{
  QJsonArray jarr;

  /* collect json items */
  for (const auto &item : _list)
    jarr.append (QJsonObject (item));

  /* store result json array */
  QByteArray result = QJsonDocument (jarr).toJson (QJsonDocument::JsonFormat::Compact);
  if (m_pCmdDataLocal != nullptr)
  {
    m_isFromSend = true;
    m_pCmdDataLocal->sendValue(DapDataLocal::NODE_ORDER_HISTORY, result);
  }
}

void DapNodeTransactionHistory::itemUpdated (int a_index)
{
  emit dataChanged (index (a_index), index (a_index));
  _delayedSave();
}

void DapNodeTransactionHistory::setCmdDataLocal(DapCmdDataLocal * pCmdDataLocal)
{
    m_pCmdDataLocal = pCmdDataLocal;
    QObject::connect(m_pCmdDataLocal, &DapCmdDataLocal::sigGotValue, [this](QString name, QVariant value, const int a_msgId) {
        Q_UNUSED(a_msgId);
        if (name == DapDataLocal::NODE_ORDER_HISTORY && !m_isFromSend)
        {
            beginResetModel();

            auto source = value.toByteArray();
            QJsonArray jarr  = QJsonDocument::fromJson (source).array();

            /* prepare parsing */
            _list.clear();

            /* parse json array */
            for (const auto &jitem : qAsConst (jarr))
            {
                DapNodeTransactionHistory::Transaction item;
                item.fromJson (jitem.toObject());
                _list.append (std::move (item));
            }

            endResetModel();

            emit listUpdated();
        }
    });
}

void DapNodeTransactionHistory::_delayedSave() const
{
  static QTimer *timer  = nullptr;

  if (timer == nullptr)
  {
    timer = new QTimer;
    timer->setSingleShot (true);
    timer->setInterval (2000);

    connect (timer, &QTimer::timeout,
             this, [this] { save(); });
  }

  timer->start();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapNodeTransactionHistory::rowCount (const QModelIndex &) const
{
  return size();
}

QVariant DapNodeTransactionHistory::data (const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  auto &item  = at (index.row());
  //qDebug().nospace() << "DapNodeTransactionHistory::data fid:" << int(role);

  switch (FieldId (role))
    {
      case FieldId::ipAddress:  return item.ipAddress;
      case FieldId::network:    return item.network;
      case FieldId::wallet:     return item.wallet;
      case FieldId::token:      return item.token;
      case FieldId::tokenValue: return item.tokenValue;
      case FieldId::unit:       return item.unit;
      case FieldId::unitValue:  return item.unitValue;
      case FieldId::priceValue: return item.priceValue;
      case FieldId::portions:   return item.portions;

      case FieldId::totalValue: return item.totalValue;
      case FieldId::created:    return item.created.toString ("hh:mm:ss dd.MM.yyyy");
      case FieldId::isSigned:   return item.isSigned;

      default: break;
    }

  return item.info.value (s_roles.value (role));
}

QHash<int, QByteArray> DapNodeTransactionHistory::roleNames() const
{
  return s_roles;
}

/********************************************
 * OPERATORS
 *******************************************/

DapNodeTransactionHistory::Transaction &DapNodeTransactionHistory::operator[] (int a_index)
{
  return _list[a_index];
}

/*-----------------------------------------*/

void DapNodeTransactionHistory::Transaction::fromJson (const QJsonObject &a_obj)
{
  for (auto i = a_obj.constBegin(), e = a_obj.constEnd(); i != e; i++ )
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

      case FieldId::ipAddress:  ipAddress   = std::move (val); break;
      case FieldId::network:    network     = std::move (val); break;
      case FieldId::wallet:     wallet      = std::move (val); break;
      case FieldId::token:      token       = std::move (val); break;
      case FieldId::tokenValue: tokenValue  = std::move (val); break;
      case FieldId::unit:       unit        = std::move (val); break;
      case FieldId::unitValue:  unitValue   = std::move (val); break;
      case FieldId::priceValue: priceValue  = std::move (val); break;
      case FieldId::portions:   portions    = std::move (val); break;

      case FieldId::totalValue: totalValue  = std::move (val); break;
      case FieldId::created:    created   = QDateTime::fromString (val, "hh:mm:ss dd.MM.yyyy"); break;
      case FieldId::isSigned:   isSigned  = i.value().toBool(); break;
    }
  }
}

QJsonObject DapNodeTransactionHistory::Transaction::toJsonObject() const
{
  return QJsonObject {
    { "info",       info.toJsonObject() },
    { "ipAddress",  ipAddress },
    { "network",    network },
    { "wallet",     wallet },
    { "token",      token },
    { "tokenValue", tokenValue },
    { "unit",       unit },
    { "unitValue",  unitValue },
    { "priceValue", priceValue },
    { "portions",   portions },

    { "totalValue", totalValue },
    { "created",    created.toString ("hh:mm:ss dd.MM.yyyy") },
    { "isSigned",   isSigned },
  };
}

/*-----------------------------------------*/
