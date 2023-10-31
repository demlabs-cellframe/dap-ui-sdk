/* INCLUDES */

#include "DapNodeWalletData.h"

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapNodeWalletData::DapNodeWalletData()
{
  _data.currentNetwork  = -1;
  _data.currentWallet   = -1;
  _data.currentToken    = -1;
}

DapNodeWalletData::~DapNodeWalletData()
{

}

/********************************************
 * METHODS
 *******************************************/

DapNodeWalletData *DapNodeWalletData::instance()
{
  static DapNodeWalletData i;
  return &i;
}

void DapNodeWalletData::setWalletsData (const QJsonObject &a_walletsData)
{
  _parseWallets (a_walletsData);
//  _prepareWalletsWithTokens();
//  _prepareNetworksWithTokens();
//  _prepareTokensAmounts();
}

void DapNodeWalletData::setOrderListData (const QJsonArray &a_ordesListData)
{

}

//const QStringList &DapNodeWalletData::wallets() const
//{
//  return _fast.walletNames;
//}

//const QMap<QString, QStringList> &DapNodeWalletData::walletsWithTokens() const
//{
//  return _fast.walletsWithTokens;
//}

//const QStringList &DapNodeWalletData::walletNetworks (const QString &a_walletName) const
//{
//  static const QStringList dummy;
//  int index = _fast.walletNames.indexOf (a_walletName);
//  if (index == -1)
//    return dummy;
//  return _wallets.at (index).fast.networkNames;
//}

//const QMap<QString, QStringList> &DapNodeWalletData::networkWithTokens (const QString &a_walletName) const
//{
//  static const QMap<QString, QStringList> dummy;
//  int index = _fast.walletNames.indexOf (a_walletName);
//  if (index == -1)
//    return dummy;
//  return _wallets.at (index).fast.networksWithTokens;
//}

//const QMap<QString, QString> &DapNodeWalletData::tokensAmount (const QString &a_walletName, const QString &a_networkName) const
//{
//  static const QMap<QString, QString> dummy;

//  /* check wallet */
//  int walletIndex     = _fast.walletNames.indexOf (a_walletName);
//  if (walletIndex == -1)
//    return dummy;

//  /* get wallet */
//  const auto &wallet  = _wallets.at (walletIndex);

//  /* check network */
//  int networkIndex    = wallet.fast.networkNames.indexOf (a_networkName);
//  if (networkIndex == -1)
//    return dummy;

//  /* get network */
//  const auto &network = wallet.networks.at (networkIndex);

//  return network.fast.tokensAmounts;
//}

const QStringList &DapNodeWalletData::networkList() const
{
  return _data.networkList;
}

const QList<DapNodeWalletDataStruct::WalletToken> &DapNodeWalletData::walletTokenList() const
{
  return _data.walletTokenList;
}

const QList<DapNodeWalletDataStruct::TokenBalance> &DapNodeWalletData::tokenBalanceList() const
{
  return _data.tokenBalanceList;
}

int DapNodeWalletData::currentNetwork() const
{
  return _data.currentNetwork;
}

void DapNodeWalletData::setCurrentNetwork (int a_value)
{
  if (_data.currentNetwork == a_value)
    return;

  _data.currentNetwork  = a_value;
  emit sigCurrentNetworkChanged();
}

int DapNodeWalletData::currentWallet() const
{
  return _data.currentWallet;
}

void DapNodeWalletData::setCurrentWallet (int a_value)
{
  if (_data.currentWallet == a_value)
    return;

  _data.currentWallet  = a_value;
  emit sigCurrentWalletChanged();
}

int DapNodeWalletData::currentToken() const
{
  return _data.currentToken;
}

void DapNodeWalletData::setCurrentToken (int a_value)
{
  if (_data.currentToken == a_value)
    return;

  _data.currentToken  = a_value;
  emit sigCurrentTokenChanged();
}

/*-----------------------------------------*/

void DapNodeWalletData::_parseWallets (const QJsonObject &a_data)
{
  _wallets.clear();
//  _fast.walletNames.clear();

  for (auto i = a_data.constBegin(), e = a_data.constEnd(); i != e; i++)
    {
      Wallet wallet;

      /* fill with data */
      wallet.name = i.key();
      _parseNetworks (i.value().toArray(), wallet);

      /* parse result data */
      _parseWalletNetworkTokenData (wallet);

      /* store name and data */
//    _fast.walletNames.append (wallet.name);
      _wallets.append (std::move (wallet));
    }

  emit sigDataUpdated();
}

void DapNodeWalletData::_parseNetworks (const QJsonArray &a_list, Wallet &a_dest)
{
  a_dest.networks.clear();
//  a_dest.fast.networkNames.clear();

  for (auto i = a_list.constBegin(), e = a_list.constEnd(); i != e; i++)
    {
      Network network;
      QJsonObject data  = i->toObject();

      /* fill with data */
      network.name    = data.value ("network").toString();
      network.address = data.value ("address").toString();
      _parseTokens (data.value ("tokens").toArray(), network/*, a_dest.fast.tokenNames*/);

      /* store name and data */
//    a_dest.fast.networkNames.append (network.name);
      a_dest.networks.append (std::move (network));
    }
}

void DapNodeWalletData::_parseTokens (const QJsonArray &a_list, Network &a_dest/*, QStringList &a_tokenNames*/)
{
  a_dest.tokens.clear();
//  a_dest.fast.tokenNames.clear();

  for (auto i = a_list.constBegin(), e = a_list.constEnd(); i != e; i++)
    {
      Token token;
      QJsonObject data  = i->toObject();

      /* fill with data */
      token.name    = data.value ("tokenName").toString();
      if (token.name == "0") // skip if empty
        continue;
      token.balance = data.value ("balance").toString();
      token.datoshi = data.value ("datoshi").toString();

      /* store name and data */
//    a_tokenNames.append (token.name);
//    a_dest.fast.tokenNames.append (token.name);
      a_dest.tokens.append (std::move (token));
    }
}

//void DapNodeWalletData::_prepareWalletsWithTokens()
//{
//  /* clear list */
//  _fast.walletsWithTokens.clear();

//  /* collect wallet tokens names */
//  for (auto i = _wallets.cbegin(), e = _wallets.cend(); i != e; i++)
//    _fast.walletsWithTokens.insert (i->name, i->fast.tokenNames);
//}

//void DapNodeWalletData::_prepareNetworksWithTokens()
//{
//  /* cycle thru wallets */
//  for (auto walletIt = _wallets.begin(), walletEnd = _wallets.end(); walletIt != walletEnd; walletIt++)
//  {
//    /* references */
//    auto &networksWithTokens  = walletIt->fast.networksWithTokens;
//    auto &networks            = walletIt->networks;

//    /* clear list */
//    networksWithTokens.clear();

//    /* collect tokens to names */
//    for (auto netIt = networks.cbegin(), netEnd = networks.cend(); netIt != netEnd; netIt++)
//      networksWithTokens.insert (netIt->name, netIt->fast.tokenNames);
//  }
//}

//void DapNodeWalletData::_prepareTokensAmounts()
//{
//  /* cycle thru wallets */
//  for (auto walletIt = _wallets.begin(), walletEnd = _wallets.end(); walletIt != walletEnd; walletIt++)
//  {
//    /* references */
//    auto &networks            = walletIt->networks;

//    /* cycle thru networks */
//    for (auto netIt = networks.begin(), netEnd = networks.end(); netIt != netEnd; netIt++)
//    {
//      /* references */
//      auto &tokensAmounts = netIt->fast.tokensAmounts;
//      auto &tokens        = netIt->tokens;

//      /* clear list */
//      tokensAmounts.clear();

//      /* collect tokens amounts */
//      for (auto tokenIt = tokens.cbegin(), tokenEnd = tokens.cend(); tokenIt != tokenEnd; tokenIt++)
//        tokensAmounts.insert (tokenIt->name, tokenIt->balance);
//    }
//  }
//}

void DapNodeWalletData::_parseWalletNetworkTokenData (const Wallet &a_value)
{
  /* lambda's */

  auto lParseTokens = [ = ] (const Wallet & a_wallet, const Network & a_network)
  {
    for (const auto &token : a_network.tokens)
      {
        /* store new WalletToken */
        _data.walletTokenList
            << DapNodeWalletDataStruct::WalletToken { a_network.name, a_wallet.name, token.name };

        /* store new TokenBalace */
        _data.tokenBalanceList
            << DapNodeWalletDataStruct::TokenBalance { a_network.name, a_wallet.name, token.name, token.balance };
      }
  };

  auto lParseNetworks = [ = ] (const Wallet & a_wallet)
  {
    for (const auto &network : a_wallet.networks)
      {
        /* parse root data */
        lParseTokens (a_wallet, network);

        /* store new network name */
        if (!_data.networkList.contains (network.name))
          _data.networkList << network.name;
      }
  };

  /* perform parsing */
  lParseNetworks (a_value);
}

/*-----------------------------------------*/
