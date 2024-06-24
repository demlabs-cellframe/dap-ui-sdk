#ifndef DAPQMLMODELNODETRANSACTIONOVERVIEW_H
#define DAPQMLMODELNODETRANSACTIONOVERVIEW_H

/* INCLUDES */

#include <QAbstractTableModel>

/****************************************//**
 * @brief node transaction overview model
 * @ingroup groupUiModels
 * @date 14.06.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelNodeTransactionOverview : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
public:
  enum class FieldId
  {
    ServerLocation,
    Address,
    Network,
    PaymentWallet,
    AvailableBalance,
    Unit,
    UnitPricePortion,
    PaymentPortions,
    FeePerPortion,
    _separator1,
    AutoRenewal,
    AutoRenewalUnits,
    AutoRenewalControl,
    _separator2,
    FeePerPortion2,
    TotalFee,
    _separator3,
    Total,

    SIZE
  };
  Q_ENUM(FieldId)

  enum class FieldType
  {
    NormalValue,
    ButtonValue,
    ValueWithButton,
    Separator,
    Button,
    TotalNameAndValue,
    NormalValueTooltip,
  };
  Q_ENUM(FieldType)

  enum class Field
  {
    id,
    name,
    value,
    type,
    icon,
    tooltip,
  };
  Q_ENUM(Field)
  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  struct Private; Private *p;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapQmlModelNodeTransactionOverview (QObject *a_parent = nullptr);
  ~DapQmlModelNodeTransactionOverview();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:

  /* basic methods */

  static DapQmlModelNodeTransactionOverview *instance();
  void refresh();

  /* fields */

  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE bool setCurrentIndex (int a_index);

  Q_INVOKABLE QVariant value (FieldId a_id) const;
  Q_INVOKABLE void setValue (FieldId a_id, const QVariant &a_value);

  Q_INVOKABLE QVariant tooltip (FieldId a_id) const;
  Q_INVOKABLE void setTooltip (FieldId a_id, const QVariant &a_value);

  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  int columnCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigCurrentIndexChanged();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELNODETRANSACTIONOVERVIEW_H
