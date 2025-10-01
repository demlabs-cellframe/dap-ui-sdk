/* INCLUDES */

#include "dapqmlmodelnodeorderlist.h"
#include "dapqmlmodelnodenetworkslist.h"
#include "DapCmdNode.h"

#include <QQueue>
#include <QJsonArray>
#include <QJsonObject>

/********************************************
 * DEFS
 *******************************************/

#define DEBUG_MSG qDebug() << "DapQmlModelOrderList" << __func__

typedef DapQmlModelNode::FieldId FieldId;
typedef DapQmlModelNode::OrderItem OrderItem;

/*-----------------------------------------*/

struct DapQmlModelNodeOrderList::Private
{
  QVector<OrderItem> items;
  int currentIndex = -1;
  QQueue<QString> feeRequestQueue;
  QString currentOrderHashCopy;
  QString unitName;
  qint64 multiplier = 1;
};

/********************************************
 * VARIABLES
 *******************************************/

QHash<int, QByteArray> DapQmlModelNode::nodeFieldsMap =
{
  { int (FieldId::location),    "location" },
  { int (FieldId::node_addr),   "node_addr" },
  { int (FieldId::price),       "price" },
  { int (FieldId::priceShort),  "priceShort" },
  { int (FieldId::units),       "units" },
  { int (FieldId::units_value), "units_value" },
  { int (FieldId::hash),        "hash" },
  { int (FieldId::ipAddress),   "ipAddress" },

  { int (FieldId::name),        "name" },
  { int (FieldId::value),       "value" },
  { int (FieldId::misc),        "misc" },

  { int (FieldId::network),     "network" },
  { int (FieldId::server),      "server" },
  { int (FieldId::wallet),      "wallet" },
};

static DapQmlModelNodeOrderList *s_instance  = nullptr;
static QRegularExpression scopesRegExp ("\\(([^)]+)\\)");
static QString s_dummyString;

/********************************************
 * FUNCTIONS
 *******************************************/

namespace NodeOrderList { QString scopedPrice (const QString &a_value); };

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelNodeOrderList::DapQmlModelNodeOrderList (QObject *a_parent)
  : QAbstractListModel (a_parent)
  , p (new Private)
{

}

DapQmlModelNodeOrderList::~DapQmlModelNodeOrderList()
{
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelNodeOrderList *DapQmlModelNodeOrderList::instance()
{
  if (s_instance == nullptr)
    s_instance = new DapQmlModelNodeOrderList;
  return s_instance;
}

QObject *DapQmlModelNodeOrderList::singletonProvider (QQmlEngine *, QJSEngine *)
{
  return instance();
}

int DapQmlModelNodeOrderList::indexOf (const QString &a_location)
{
  /* iterate in cycle */
  int index = 0;
  for (auto i = p->items.cbegin(), e = p->items.cend(); i != e; i++, index++)
    if (i->location == a_location)
      return index;

  return -1;
}

void DapQmlModelNodeOrderList::refresh()
{
  _modelReset();
}

int DapQmlModelNodeOrderList::length() const
{
  return p->items.size();
}

const QString &DapQmlModelNodeOrderList::hash() const
{
  if (p->currentIndex < 0 || p->currentIndex >= p->items.size())
    return s_dummyString;
  return p->items.at (p->currentIndex).hash;
}

int DapQmlModelNodeOrderList::currentIndex() const
{
  return p->currentIndex;
}

void DapQmlModelNodeOrderList::setCurrentIndex (int a_index)
{
  if (p->currentIndex == a_index)
    return;

  p->currentIndex = a_index;
  emit sigCurrentIndexChanged();

  if (p->currentIndex >= 0
      && p->currentIndex < p->items.size())
    emit sigOrderSelected (p->items.at (p->currentIndex).hash);
}

const QVector<DapQmlModelNode::OrderItem> &DapQmlModelNodeOrderList::orders() const
{
  return p->items;
}

void DapQmlModelNodeOrderList::setOrders (const QVector<DapQmlModelNode::OrderItem> &a_value)
{
  beginResetModel();
  p->items  = a_value;
  endResetModel();
}

void DapQmlModelNodeOrderList::setOrders (QVector<DapQmlModelNode::OrderItem> &&a_value)
{
  beginResetModel();
  p->items  = std::move (a_value);
  endResetModel();
}

const DapQmlModelNode::OrderItem *DapQmlModelNodeOrderList::currentOrder() const
{
  static OrderItem dummy;
  auto index  = currentIndex();
  if (p->items.isEmpty()
      || index < 0
      || index >= p->items.size())
    return &dummy;
  return &p->items.at (index);
}

void DapQmlModelNodeOrderList::setOrderListData (const QJsonArray &a_list, bool notify)
{
  //  if (a_list.isEmpty())
  //    return;

  /* notify model */
  beginResetModel();

  /* vars */
  QVector<OrderItem> items;
  QSet<QString> addressesSet;
  QJsonArray jarray;

  if (!hash().isEmpty())
    p->currentOrderHashCopy = hash();
  int newCurrentIndex = -1, index = 0;

  /* parse via cycle */
  for (const auto &item : qAsConst (a_list))
    {
      /* get item */
      QJsonObject joItem = item.toObject();
      if (!joItem.contains ("node_location"))
        continue;

      /* get fields */
      QString loc        = joItem.value ("node_location").toString();
      QString price      = joItem.value ("price").toString();
      QString punit      = joItem.value ("price_unit").toString();
      QString unit_value = joItem.value ("units").toString();
      QString server     = joItem.value ("node_location").toString();
      QString node_addr  = joItem.value ("node_addr").toString();
      QString hash       = joItem.value ("hash").toString();
      QString srv_uid    = joItem.value ("srv_uid").toString();

      /* check */
      if (srv_uid != "0x0000000000000001"
          || unit_value == "0"
          || punit.isEmpty())
        continue;

      /* update new current */
      if (!p->currentOrderHashCopy.isEmpty()
          && hash  == p->currentOrderHashCopy)
      {
        p->currentOrderHashCopy.clear();
        newCurrentIndex = index;
      }

      /* store result */
      addressesSet << node_addr;
      items << OrderItem
      {
        std::move (loc),
        std::move (price),
        std::move (punit),
        std::move (unit_value),
        std::move (server),
        std::move (node_addr),
        std::move (hash),
        QString()
      };
      index++;
    }

  /* store result */
  setOrders (std::move (items));
  setCurrentIndex (newCurrentIndex);

  /* notify model */
  endResetModel();

  /* collect addresses into json */
  for (const auto &address : qAsConst (addressesSet))
    jarray << address;

  /* request ip's */
  if (!jarray.isEmpty())
    emit sigRequestNodeIPs (DapQmlModelNodeNetworksList::instance()->network(), jarray);

  /* finished */
  if (notify)
    emit sigOrderListLoaded();
}

void DapQmlModelNodeOrderList::installAddressMap (const QHash<QString, QString> &a_map)
{
  for (auto i = p->items.begin(), e = p->items.end(); i != e; i++)
    i->ipAddress  = a_map.value (i->node_addr, QString());
}

void DapQmlModelNodeOrderList::_modelReset()
{
  beginResetModel();
  endResetModel();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelNodeOrderList::rowCount (const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return p->items.size();
}

int DapQmlModelNodeOrderList::columnCount (const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return DapQmlModelNode::nodeFieldsMap.size();
}

QVariant DapQmlModelNodeOrderList::data (const QModelIndex &index, int role) const
{
  if (index.isValid() && index.row() < p->items.size())
    {
      const auto &item  = p->items.at (index.row());
      switch (FieldId (role))
        {
        case FieldId::location:    return item.location;
        case FieldId::node_addr:   return item.node_addr;
        case FieldId::price:       return item.price;
        case FieldId::priceShort:  return NodeOrderList::scopedPrice (item.price);
        case FieldId::units:
        {
          if (p->unitName.isEmpty())
            return item.units;
          else
            return p->unitName;
        }

        case FieldId::units_value:
        {
          if (p->unitName.isEmpty())
            return item.units_value;
          else
            return (p->multiplier > 1)
                ? QString::number (item.units_value.toULongLong() / p->multiplier)
                : (item.units_value);
        }

        case FieldId::server:      return item.server;
        case FieldId::hash:        return item.hash;
        case FieldId::ipAddress:   return item.ipAddress;

        default:
          break;
        }
    }
  return QVariant();
}

QHash<int, QByteArray> DapQmlModelNodeOrderList::roleNames() const
{
  return DapQmlModelNode::nodeFieldsMap;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelNodeOrderList::slotSetData (const QJsonArray &a_list)
{
  setOrderListData (a_list);
}

void DapQmlModelNodeOrderList::slotSetOrderAddresses (const QJsonObject &a_list)
{
  beginResetModel();

  {
    int index = 0;
    QHash<QString, QString> nodeIpsMap;

    for (auto i = a_list.constBegin(), e = a_list.constEnd(); i != e; i++, index++)
      nodeIpsMap.insert (i.key(), i.value().toString());

    installAddressMap (nodeIpsMap);
  }

  endResetModel();
}




/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelOrderListProxyModel::DapQmlModelOrderListProxyModel()
  : QSortFilterProxyModel()
  , m_unit()
  , m_min (-1)
  , m_max (-1)
{
  /* setup model */
  setSourceModel (DapQmlModelNodeOrderList::instance());
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelOrderListProxyModel::setRowFilter (const QString &a_unit, qint64 a_min, qint64 a_max)
{
  /* variables */

  auto model = DapQmlModelNodeOrderList::instance();

  /* store */

  m_unit  = a_unit == "All" ? QString() : a_unit;
  m_min   = a_min;
  m_max   = a_max;
  model->p->unitName  = m_unit.toUpper();

  /* variables */

  QString unit      = m_unit;
  qint64 minPrice   = m_min;
  qint64 maxPrice   = m_max;

  /* convert units */

  DapCmdNode::convertUnits (unit, minPrice, maxPrice, &model->p->multiplier);

  /* store */

  m_unit  = unit.toUpper();
  m_min   = minPrice;
  m_max   = maxPrice;

  invalidateFilter();
}

int DapQmlModelOrderListProxyModel::currentIndex() const
{
  return DapQmlModelNodeOrderList::instance()->currentIndex();
}

void DapQmlModelOrderListProxyModel::setCurrentIndex (int a_value)
{
  DapQmlModelNodeOrderList::instance()->setCurrentIndex (mapToSource (index (a_value, 0)).row());
  emit sigCurrentIndexChanged();
}

/********************************************
 * OVERRIDE
 *******************************************/

bool DapQmlModelOrderListProxyModel::filterAcceptsRow (
  int sourceRow, const QModelIndex &) const
{
  //DEBUG_MSG << m_unit << m_min << m_max;
  try
    {
      const QVector<OrderItem> &orders = DapQmlModelNodeOrderList::instance()->orders();

      auto order      = orders.at (sourceRow);
      qreal value     = order.units_value.toDouble();

      bool byUnit     = m_unit.isEmpty()  ? true : order.units.toUpper() == m_unit;
      bool byMin      = m_min < 0         ? true : (value >= m_min);
      bool byMax      = m_max < 0         ? true : (value <= m_max);

      //qDebug("%s unit %d min %d max %d", __func__, byUnit, byMin, byMax);
      return byUnit && byMin && byMax;
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
      return false;
    }
}




/*-----------------------------------------*/

namespace NodeOrderList {
QString scopedPrice (const QString &a_value)
{
  auto match = scopesRegExp.match (a_value);

  if (match.hasMatch())
    return match.captured (1);

  return QString();
}
};

/*-----------------------------------------*/
