#ifndef DAPNODEWALLETDATA_H
#define DAPNODEWALLETDATA_H

/* INCLUDES */

#include <QObject>
#include <QMap>

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
    QList<Token> tokens;    // network tokens

    struct // fast access
    {
      QStringList tokenNames; // token names
      QMap<QString, QString> tokensAmounts; // tokens amounts
    } fast;
  };

  struct Wallet
  {
    QString name;             // wallet name
    QList<Network> networks;  // wallet networks

    struct // fast access
    {
      QStringList networkNames; // wallet networks
      QStringList tokenNames;   // all token names
      QMap<QString, QStringList> networksWithTokens;
    } fast;
  };

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QList<Wallet> _wallets;   // all wallets

  struct // fast access
  {
    QStringList walletNames; // wallet names
    QMap<QString, QStringList> walletsWithTokens;  // wallets with tokens
  } _fast;
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
  void setData (const QJsonObject &a_walletsData);

  /// get wallets names list
  const QStringList &wallets() const;

  /// get map with wallet names and corresponded tokens lists
  const QMap<QString, QStringList> &walletsWithTokens() const;

  /// get wallet network names list
  const QStringList &walletNetworks (const QString &a_walletName) const;

  /// get map with network names and corresponded tokens list
  const QMap<QString, QStringList> &networkWithTokens (const QString &a_walletName) const;

  /// get map with token names and balances
  const QMap<QString, QString> &tokensAmount (const QString &a_walletName, const QString &a_networkName) const;

protected:
  void _parseWallets (const QJsonObject &a_data);
  void _parseNetworks (const QJsonArray &a_list, Wallet &a_dest);
  void _parseTokens (const QJsonArray &a_list, Network &a_dest, QStringList &a_tokenNames);

  void _prepareWalletsWithTokens();
  void _prepareNetworksWithTokens();
  void _prepareTokensAmounts();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigDataUpdated();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNODEWALLETDATA_H
