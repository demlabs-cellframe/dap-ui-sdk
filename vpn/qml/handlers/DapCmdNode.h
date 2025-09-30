#ifndef DAPCMDNODEHANDLER_H
#define DAPCMDNODEHANDLER_H

/* INCLUDES */
#include <QObject>
#include "DapCmdClientAbstract.h"
#include "DapNodeOrderInfo.h"

/****************************************//**
 * @brief order list interface
 *
 * The class implements the command handler
 * for receiving user data from the service
 *
 * @date 22.07.2023
 * @author Stanislav Ratseburzhinskii, Mikhail Shilenko
 *******************************************/

class DapCmdNode: public DapCmdClientAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  struct DapCmdNodeData;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  DapCmdNodeData *_data;
  
  // Store connected app type (Dashboard or Cellframe-Wallet)
  QString m_connectedAppType = "Dashboard";
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapCmdNode (QObject *parent = nullptr);
  virtual ~DapCmdNode() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void startNodeDetection();
  void stopCheckNode();
  void noCdbModeRequest();
  void walletsRequest();

  bool hasError();
//  WalletsData *nodeInfo() { return &m_dataWallet; }
  DapNodeOrderInfo orderData (const QString &hash);
  /// convert units from kilo (mega etc) to byte. same for seconds
  static void convertUnits (QString &a_unit, qint64 &a_min, qint64 &a_max, qint64 *a_multiplier = nullptr);

  /// Get connected app type (Dashboard or Cellframe-Wallet)
  QString getConnectedAppType() const;
  
  /// Set connected app type when detected by DapNode
  void setConnectedAppType(const QString& appType);

protected:
  bool _checkContinue();
  void _updateHistoryItem();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  /// Process command.
  /// @param params Command parameters.
  void handleResult (const QJsonObject &result) override;
  void handleError (int code, const QString &message) override;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotWalletsDataRequest();
  void slotNodeDetection();
  void slotChooseNetwork (const QString &network);
  void slotChooseWallet (const QString &wallet);
  void slotChooseToken (const QString &token);
  void slotSetValue (const QString &value);
  void slotSetUnit (const QString &unit);
  void slotChooseOrder (const QString &hash);
  void slotSetTransactionInfo (const QVariant &a_valueMap);
  void slotCondTxCreate();
  void slotStartSearchOrders();
  void slotSetMaxValueUnit (const QString &price);
  void slotSetMinValueUnit (const QString &price);
  void slotCheckSigned();
  void slotStartConnectByOrder();
  void slotStartConnectByHistoryOrder (const DapNodeOrderInfo &a_info, const QString &a_token, const QString &a_network);
  void slotRequestIpNode (const QString &networkName, const QJsonArray &orderList);
  void slotRequestNetworkFee (const QString &a_networkName);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
//  void sigWalletsList (QHash<QString, QStringList> data);
//  void sigNetworksList (QHash<QString, QStringList> data);
//  void sigTokensInfo (QHash<QString, QString> data);
//  void sigOrderListData (QMap<QString, QVariant> data);
//  void sigTokenAmount (QString token, QString amount);
//  void sigWalletsList (QStringList a_list);
  void sigNodeError (int code, QString errorMessage);
  void sigTransactionHashInMempool();
  void sigTransactionHashInledger();
  void sigTransactionInQueue(const QString& idQueue, const QString& appType);
  void sigContinueEnable (bool);
  void sigSigningReceived (qint32 utype, qint64 uid, QString units, QString value);
  void sigOrderList (const QJsonArray &a_list);
  void sigNodeIpList (const QJsonObject &a_list);
  void sigFeeData (const QJsonObject &a_data);

  void sigNodeDetected();
  void sigOrderListDataUpdated();

  /// wallets, networks and tokens updated
  void sigWalletsDataUpdated();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDNODEHANDLER_H
