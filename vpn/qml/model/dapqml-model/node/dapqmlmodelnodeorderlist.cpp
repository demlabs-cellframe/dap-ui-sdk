/* INCLUDES */

#include "dapqmlmodelnodeorderlist.h"

#include <QQueue>

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

int DapQmlModelNodeOrderList::length() const
{
  return p->items.size();
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
  emit sigOrderSelected (p->items.at (p->currentIndex).hash);
}

const DapQmlModelNode::OrderItem *DapQmlModelNodeOrderList::currentOrder() const
{

}

void DapQmlModelNodeOrderList::setOrderListData (const QJsonArray &a_list, bool notify)
{

}

void DapQmlModelNodeOrderList::_modelReset()
{

}

void DapQmlModelNodeOrderList::_setNetworksFeeRequestList (const QStringList &a_list)
{

}

QString DapQmlModelNodeOrderList::_dequeueNetworkFeeRequest()
{

}

void DapQmlModelNodeOrderList::_setNetworkFee (const QString &a_networkName, const QString &a_fee)
{

}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelNodeOrderList::rowCount (const QModelIndex &parent) const
{

}

int DapQmlModelNodeOrderList::columnCount (const QModelIndex &parent) const
{

}

QVariant DapQmlModelNodeOrderList::data (const QModelIndex &index, int role) const
{

}

QHash<int, QByteArray> DapQmlModelNodeOrderList::roleNames() const
{

}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelNodeOrderList::slotSetData (const QJsonArray &a_list)
{

}

void DapQmlModelNodeOrderList::slotWalletsDataUpdated()
{

}

void DapQmlModelNodeOrderList::slotSetOrderAddresses (const QJsonObject &a_list)
{

}

/*-----------------------------------------*/
