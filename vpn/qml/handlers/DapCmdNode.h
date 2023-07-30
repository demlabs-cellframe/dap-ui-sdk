#ifndef DAPCMDNODEHANDLER_H
#define DAPCMDNODEHANDLER_H

/* INCLUDES */
#include <QObject>
#include "DapCmdClientAbstract.h"

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
public:
  struct OrderInfo
  {
    QString direction;
    QString ext;
    QString hash;
    QString nodeAddress;
    QString nodeLocation;
    QString pkey;
    QString price;
    QString priceUnit;
    QString srvUid;
    QString txCondHash;
    QString units;
    QString version;

    static OrderInfo fromJson (const QJsonObject &a_obj);
    QJsonObject toJsonObject() const;
  };

  typedef QList<OrderInfo> OrderInfoList;
  typedef QHash<QString, OrderInfo> OrderInfoMap;

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
  OrderInfo orderData (QString hash);
  void checkSigned();
  void startConnectByOrder();
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
};

/*-----------------------------------------*/
#endif // DAPCMDNODEHANDLER_H
