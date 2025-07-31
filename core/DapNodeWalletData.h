#ifndef DAPNODEWALLETDATA_H
#define DAPNODEWALLETDATA_H

/* INCLUDES */

#include <QObject>
#include <QMap>

/* DEFS */

namespace DapNodeWalletDataStruct {

struct WalletToken
{
  QString network;
  QString wallet;   // name
  QString token;
  QString balance;
};

struct TokenBalance
{
  QString network;
  QString wallet;
  QString token;    // name
  QString balance;
};

struct NetworkFee
{
  QString network;    // name
  QString feeTicker;  // token
  QString feeValue;   // price
};

};

/****************************************//**
 * @brief node wallet data
 *
 * The class provides an easy way to handle
 * wallet data, such as tokens, networks, etc.
 *
 * @date 26.10.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapNodeWalletData : public QObject
{
  Q_OBJECT

  Q_DISABLE_COPY_MOVE(DapNodeWalletData)

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  Q_PROPERTY (int currentNetwork  READ currentNetwork WRITE setCurrentNetwork NOTIFY sigCurrentNetworkChanged)
  Q_PROPERTY (int currentWallet   READ currentWallet  WRITE setCurrentWallet  NOTIFY sigCurrentWalletChanged)
  Q_PROPERTY (int currentToken    READ currentToken   WRITE setCurrentToken   NOTIFY sigCurrentTokenChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  struct Token
  {
    QString name;     // token name
    QString balance;  // balanse
    QString datoshi;  // datoshi price
  };

  struct Network
  {
    QString name;           // network name
    QString address;        // network address
    QString feeTicker;      // network fee ticker
    QString feeValue;       // network fee value
    QList<Token> tokens;    // network tokens

//    struct // fast access
//    {
//      QStringList tokenNames; // token names
//      QMap<QString, QString> tokensAmounts; // tokens amounts
//    } fast;
  };

  struct Wallet
  {
    QString name;             // wallet name
    QList<Network> networks;  // wallet networks

//    struct // fast access
//    {
//      QStringList networkNames; // wallet networks
//      QStringList tokenNames;   // all token names
//      QMap<QString, QStringList> networksWithTokens;
//    } fast;
  };

  struct Order
  {
    QString location;
    QString price;
    QString units;
    QString units_value;
    QString server;
    QString node_addr;
    QString hash;
    QString ipAddress;
  };

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QList<Wallet> _wallets;   // all wallets
  QList<Order> _orders;     // all orders

//  struct // fast access
//  {
//    QStringList walletNames; // wallet names
//    QMap<QString, QStringList> walletsWithTokens;  // wallets with tokens
//  } _fast;

  // model data
  struct
  {
    QStringList networkList;
    QMap<QString, DapNodeWalletDataStruct::NetworkFee> networkFeeMap;
    QList<DapNodeWalletDataStruct::WalletToken> walletTokenList;
    QList<DapNodeWalletDataStruct::TokenBalance> tokenBalanceList;

    int currentNetwork;
    int currentWallet;
    int currentToken;
  } _data;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapNodeWalletData();
  ~DapNodeWalletData();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapNodeWalletData *instance();

  /// parse json data
  void setWalletsData (const QJsonObject &a_walletsData);
  void setOrderListData (const QJsonArray &a_ordesListData);

  void setNetworkFee (const QString &a_networkName, const QString &a_fee, const QString &a_feeValue);

//  /// get wallets names list
//  const QStringList &wallets() const;

//  /// get map with wallet names and corresponded tokens lists
//  const QMap<QString, QStringList> &walletsWithTokens() const;

//  /// get wallet network names list
//  const QStringList &walletNetworks (const QString &a_walletName) const;

//  /// get map with network names and corresponded tokens list
//  const QMap<QString, QStringList> &networkWithTokens (const QString &a_walletName) const;

//  /// get map with token names and balances
//  const QMap<QString, QString> &tokensAmount (const QString &a_walletName, const QString &a_networkName) const;

  const QStringList &networkList() const;
  const QMap<QString, DapNodeWalletDataStruct::NetworkFee> &networkFeeMap() const;
  const QList<DapNodeWalletDataStruct::WalletToken> &walletTokenList() const;
  const QList<DapNodeWalletDataStruct::TokenBalance> &tokenBalanceList() const;

  int currentNetwork() const;
  void setCurrentNetwork (int a_value);

  int currentWallet() const;
  void setCurrentWallet (int a_value);

  int currentToken() const;
  void setCurrentToken (int a_value);

protected:
  void _parseWallets (const QJsonObject &a_data);
  void _parseNetworks (const QJsonArray &a_list, Wallet &a_dest);
  void _parseTokens (const QJsonArray &a_list, Network &a_dest /*, QStringList &a_tokenNames*/);

//  void _prepareWalletsWithTokens();
//  void _prepareNetworksWithTokens();
//  void _prepareTokensAmounts();

  void _parseWalletNetworkTokenData (const Wallet &a_value);
  void _printWalletsData();
  void _emitNetworksList();
  void _cleanInvalidTokens();
  void _addEmptyNativeWallets(const QString &a_networkName);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigDataUpdated();
  void sigCurrentNetworkChanged();
  void sigCurrentWalletChanged();
  void sigCurrentTokenChanged();
  void sigNetworksList (QStringList a_list);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNODEWALLETDATA_H
