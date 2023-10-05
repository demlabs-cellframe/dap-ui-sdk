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

  bool hasError();
//  WalletsData *nodeInfo() { return &m_dataWallet; }

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
  void chooseNetwork (QString network);
  void chooseWallet (QString wallet);
  void chooseToken (QString token);
  void setValue (QString value);
  void setUnit (QString unit);
  void chooseOrder (QString hash);
  void condTxCreate();
  void startSearchOrders();
  void setMaxValueUnit (QString price);
  void setMinValueUnit (QString price);
  DapNodeOrderInfo orderData (QString hash);
  void checkSigned();
  void startConnectByOrder();
  void getIpNode (const QString &networkName, const QJsonArray &orderList);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void nodeDetected();
  void walletsList (QHash<QString, QStringList> data);
  void networksList (QHash<QString, QStringList> data);
  void tokensInfo (QHash<QString, QString> data);
  void orderList (QMap<QString, QVariant> data);
  void tokenAmount (QString token, QString amount);
  void nodeError (int code, QString errorMessage);
  void transactionHashInMempool();
  void transactionHashInledger();
//    void dataWallets(WalletsData* walletsData);
  void continueEnable (bool);
  void signingReceived (qint32 utype, qint64 uid, QString units, QString value);
  void sigOrderList (const QJsonArray &a_list);
  void sigNodeIpList (const QJsonArray &a_list);
};

/*-----------------------------------------*/
#endif // DAPCMDNODEHANDLER_H
