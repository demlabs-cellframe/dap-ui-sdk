/* INCLUDES */

#include "dapqmlmodelnodetransactionoverview.h"
#include <QJsonArray>
#include <QJsonObject>

/* DEFINES */

typedef DapQmlModelNodeTransactionOverview::FieldId FieldId;
typedef DapQmlModelNodeTransactionOverview::FieldType FieldType;
typedef DapQmlModelNodeTransactionOverview::Field Field;

//struct DqmntoRow
//{
//  FieldId fid;
//  QString name;
//  QVariant value;

//  QString btnIcon;
//  QString valueColor;
//};

#define ITEM_NORMAL(a_id,a_name) \
  QJsonObject{ \
    { "id", int (a_id) }, \
    { "name", a_name }, \
    { "value", "TEST" }, \
    { "type", int (FieldType::NormalValue) }, \
  }

#define ITEM_BTN_VALUE(a_id,a_name) \
  QJsonObject{ \
    { "id", int (a_id) }, \
    { "name", a_name }, \
    { "value", "TEST" }, \
    { "type", int (FieldType::ButtonValue) }, \
  }

#define ITEM_VALUE_WITH_BTN(a_id,a_name,a_icon) \
  QJsonObject{ \
    { "id", int (a_id) }, \
    { "name", a_name }, \
    { "value", "TEST" }, \
    { "tooltip", "" }, \
    { "icon", a_icon }, \
    { "type", int (FieldType::ValueWithButton) }, \
  }

#define ITEM_SEPARATOR(a_id) \
  QJsonObject{ \
    { "id", int (a_id) }, \
    { "type", int (FieldType::Separator) }, \
  }

#define ITEM_BUTTON(a_id,a_name) \
  QJsonObject{ \
    { "id", int (a_id) }, \
    { "name", a_name }, \
    { "type", int (FieldType::Button) }, \
  }

#define ITEM_TOTAL(a_id,a_name) \
  QJsonObject{ \
    { "id", int (a_id) }, \
    { "name", a_name }, \
    { "value", "TEST" }, \
    { "tooltip", "" }, \
    { "type", int (FieldType::TotalNameAndValue) }, \
  }

#define ITEM_NORMAL_TOOLTIP(a_id,a_name) \
  QJsonObject{ \
    { "id", int (a_id) }, \
    { "name", a_name }, \
    { "value", "TEST" }, \
    { "tooltip", "" }, \
    { "type", int (FieldType::NormalValueTooltip) }, \
  }

struct DapQmlModelNodeTransactionOverview::Private
{
  int currentIndex = -1;
};

/* VARIABLES */

static DapQmlModelNodeTransactionOverview *s_instance  = nullptr;
static QJsonArray s_data =
{
  ITEM_NORMAL (FieldId::ServerLocation, "Server location:"),
  ITEM_VALUE_WITH_BTN (FieldId::Address, "IP:", "qrc:/dapqml-forms-asset/ic_trov_copy.png"),
  ITEM_NORMAL (FieldId::Network, "Network:"),
  ITEM_NORMAL (FieldId::PaymentWallet, "Payment wallet:"),
  ITEM_NORMAL_TOOLTIP (FieldId::AvailableBalance, "Available:"),
  ITEM_VALUE_WITH_BTN (FieldId::Unit, "Unit:", "qrc:/dapqml-forms-asset/ic_trov_edit.png"),
  ITEM_NORMAL_TOOLTIP (FieldId::UnitPricePortion, "Unit price portion:"),
  ITEM_NORMAL (FieldId::PaymentPortions, "Payment portions:"),

  ITEM_SEPARATOR (FieldId::_separator1),
  ITEM_NORMAL_TOOLTIP (FieldId::FeePerPortion, "Fee per portion:"),

//  ITEM_BTN_VALUE (FieldId::AutoRenewal, "Auto-renewal:"),
//  ITEM_NORMAL (FieldId::AutoRenewalUnits, "Auto-renewal units:"),
//  ITEM_BUTTON (FieldId::AutoRenewalControl, "Disable auto-renewal"),

//  ITEM_SEPARATOR (FieldId::_separator2),
//  ITEM_NORMAL (FieldId::FeePerPortion2, "Fee per portion:"),
  ITEM_NORMAL_TOOLTIP (FieldId::TotalFee, "Total fee:"),

  ITEM_SEPARATOR (FieldId::_separator3),
  ITEM_TOTAL (FieldId::Total, "Total:"),
};

static const QHash<int, QByteArray> s_roles =
{
  { int (Field::id),      "id" },
  { int (Field::name),    "name" },
  { int (Field::value),   "value" },
  { int (Field::type),    "type" },
  { int (Field::icon),    "icon" },
  { int (Field::tooltip), "tooltip" },
};

/* FUNCTIONS */

static QJsonObject _variantMapToJson (const QVariantMap &a_value);
static QVariantMap _jsonObjectToVariantMap (const QJsonObject &a_value);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelNodeTransactionOverview::DapQmlModelNodeTransactionOverview (QObject *parent)
  : QAbstractTableModel{parent}
  , p (new Private)
{
  setObjectName ("DapQmlModelNodeTransactionOverview");
}

DapQmlModelNodeTransactionOverview::~DapQmlModelNodeTransactionOverview()
{
  s_instance  = nullptr;
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelNodeTransactionOverview *DapQmlModelNodeTransactionOverview::instance()
{
  if (s_instance == nullptr)
    s_instance = new DapQmlModelNodeTransactionOverview;
  return s_instance;
}

void DapQmlModelNodeTransactionOverview::refresh()
{
  beginResetModel();
  endResetModel();
}

int DapQmlModelNodeTransactionOverview::currentIndex() const
{
  return p->currentIndex;
}

bool DapQmlModelNodeTransactionOverview::setCurrentIndex (int a_index)
{
  if (p->currentIndex == a_index)
    return false;

  p->currentIndex = a_index;
  emit sigCurrentIndexChanged();
  return true;
}

QVariant DapQmlModelNodeTransactionOverview::value (FieldId a_id) const
{
  auto item  = s_data.at (int (a_id)).toObject();
  if (item.contains ("value"))
    return item.value ("value");
  return QVariant();
//  return _jsonObjectToVariantMap (s_data.at (int (a_id)).toObject());
}

void DapQmlModelNodeTransactionOverview::setValue (FieldId a_id, const QVariant &a_value)
{
  auto item  = s_data.at (int (a_id)).toObject();
  if (!item.contains ("value"))
    return;
  item["value"] = QJsonValue::fromVariant (a_value);
  s_data[int (a_id)] = item;
}

QVariant DapQmlModelNodeTransactionOverview::tooltip (FieldId a_id) const
{
  auto item  = s_data.at (int (a_id)).toObject();
  if (item.contains ("tooltip"))
    return item.value ("tooltip");
  return QVariant();
}

void DapQmlModelNodeTransactionOverview::setTooltip (FieldId a_id, const QVariant &a_value)
{
  auto item  = s_data.at (int (a_id)).toObject();
  if (!item.contains ("tooltip"))
    return;
  item["tooltip"] = QJsonValue::fromVariant (a_value);
  s_data[int (a_id)] = item;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelNodeTransactionOverview::rowCount (const QModelIndex &) const
{
  return s_data.size();
}

int DapQmlModelNodeTransactionOverview::columnCount (const QModelIndex &) const
{
  return s_roles.size();
}

QVariant DapQmlModelNodeTransactionOverview::data (const QModelIndex &index, int role) const
{
  if (index.row() < 0 || index.row() >= s_data.size())
    return QVariant();

  if (!s_roles.contains (role))
    return QVariant();

  auto field  = Field (role);
  auto item   = s_data.at (index.row()).toObject();

  switch (field)
    {
      case Field::id:       return item.value ("id").toVariant();
      case Field::name:     return item.value ("name").toVariant();
      case Field::value:    return item.value ("value").toVariant();
      case Field::type:     return item.value ("type").toVariant();
      case Field::icon:     return item.value ("icon").toVariant();
      case Field::tooltip:  return item.value ("tooltip").toVariant();

      default:
        return QVariant();
    }
}

QHash<int, QByteArray> DapQmlModelNodeTransactionOverview::roleNames() const
{
  return s_roles;
}

/*-----------------------------------------*/

QJsonObject _variantMapToJson (const QVariantMap &a_value)
{
  QJsonObject result;
  for (auto it = a_value.begin(); it != a_value.end(); it++)
    result.insert (it.key(), QJsonValue::fromVariant (it.value()));
  return result;
}

QVariantMap _jsonObjectToVariantMap (const QJsonObject &a_value)
{
  QVariantMap result;
  for (auto it = a_value.begin(); it != a_value.end(); it++)
    result.insert (it.key(), it.value().toVariant());
  return result;
}
/*-----------------------------------------*/
