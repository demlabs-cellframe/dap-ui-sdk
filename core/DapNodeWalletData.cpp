/* INCLUDES */

#include "DapNodeWalletData.h"

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

/* DEFS */
#define PRINT_WALLET_DATA
//#define SIMULATE_DATA
#define DEBUGINFO qDebug() << (QString ("DapNodeWalletData::") + __func__)
#define ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))

/* VARS */
#ifdef SIMULATE_DATA

static const char* const s_simNetworks[] =
{
  "riemann",
  "subzero",
  "KelVPN",
  "Backbone",
};

static const char* const s_simWallets[] =
{
  "riemann:Brad:tKEL",
  "KelVPN:Brad:KEL",
  "KelVPN:Brad:CELL",
  "KelVPN:Brad:mKEL",
  "Backbone:Brad:CELL",
  "Backbone:Brad:mCELL",
  "Backbone:Brad:KEL",
  "riemann:Jax:tKEL",
  "KelVPN:Jax:KEL",
  "Backbone:Jax:CELL",
  "Backbone:Jax:mCELL",
  "Backbone:Jax:KEL",
  "Backbone:Jax:mKEL",
  "riemann:Jim:tKEL",
  "subzero:Jim:tCELL",
  "KelVPN:Jim:KEL",
  "KelVPN:Jim:CELL",
  "Backbone:Jim:CELL",
  "Backbone:Jim:KEL",
  "riemann:Ksu:tKEL",
  "riemann:Ksu:mtKEL",
  "KelVPN:Ksu:KEL",
  "KelVPN:Ksu:CELL",
  "KelVPN:Ksu:mKEL",
  "Backbone:Ksu:CELL",
  "Backbone:Ksu:mCELL",
  "Backbone:Ksu:KEL",
  "Backbone:Ksu:mKEL",
  "riemann:Ksy:tKEL",
  "subzero:Ksy:tCELL",
  "KelVPN:Ksy:KEL",
  "Backbone:Ksy:CELL",
  "riemann:Max:tKEL",
  "subzero:Max:tCELL",
  "KelVPN:Max:KEL",
  "Backbone:Max:KEL",
  "Backbone:Max:CELL",
  "riemann:One:tKEL",
  "riemann:One:mtKEL",
  "subzero:One:tCELL",
  "subzero:One:mtCELL",
  "KelVPN:One:KEL",
  "Backbone:One:CELL",
  "riemann:Pa:tKEL",
  "riemann:Seven:tKEL",
  "subzero:Seven:tCELL",
  "riemann:Six:tKEL",
  "riemann:Sofa:tKEL",
  "subzero:Sofa:tCELL",
  "subzero:Sofa:mtCELL",
  "riemann:Vit:tKEL",
  "subzero:Vit:tCELL",
  "KelVPN:Vit:KEL",
  "KelVPN:Vit:mKEL",
  "KelVPN:Vit:CELL",
  "Backbone:Vit:CELL",
  "Backbone:Vit:KEL",
  "Backbone:Vit:mCELL",
  "riemann:Vita:tKEL",
  "KelVPN:Vita:KEL",
  "KelVPN:Vita:CELL",
  "Backbone:Vita:CELL",
  "Backbone:Vita:mCELL",
  "Backbone:Vita:KEL",
  "riemann:blade:tKEL",
  "KelVPN:blade:KEL",
  "KelVPN:blade:mKEL",
  "KelVPN:blade:CELL",
  "Backbone:blade:KEL",
  "Backbone:blade:CELL",
  "Backbone:blade:mCELL",
  "Backbone:blade:mKEL",
  "riemann:maka:tKEL",
  "KelVPN:maka:KEL",
  "KelVPN:maka:CELL",
  "Backbone:maka:KEL",
  "Backbone:maka:CELL",
};

static const char* const s_simTokens[] =
{
  "riemann:Brad:tKEL:0.0",
  "KelVPN:Brad:KEL:4.85",
  "KelVPN:Brad:CELL:1.0",
  "KelVPN:Brad:mKEL:0.0015",
  "Backbone:Brad:CELL:0.444727427518015755",
  "Backbone:Brad:mCELL:0.0063",
  "Backbone:Brad:KEL:0.9",
  "riemann:Jax:tKEL:1939.4",
  "KelVPN:Jax:KEL:1.65",
  "Backbone:Jax:CELL:2.629314547380662817",
  "Backbone:Jax:mCELL:0.00491",
  "Backbone:Jax:KEL:0.3",
  "Backbone:Jax:mKEL:0.0002",
  "riemann:Jim:tKEL:15555.74",
  "subzero:Jim:tCELL:1992.2",
  "KelVPN:Jim:KEL:7.3",
  "KelVPN:Jim:CELL:1.0",
  "Backbone:Jim:CELL:4.15",
  "Backbone:Jim:KEL:1.0",
  "riemann:Ksu:tKEL:233802.152142857142857144",
  "riemann:Ksu:mtKEL:0.2",
  "KelVPN:Ksu:KEL:7.967658520391762081",
  "KelVPN:Ksu:CELL:1.0",
  "KelVPN:Ksu:mKEL:0.0024",
  "Backbone:Ksu:CELL:20.131976332556031378",
  "Backbone:Ksu:mCELL:0.006856990029909977",
  "Backbone:Ksu:KEL:895.470567377507341821",
  "Backbone:Ksu:mKEL:0.087",
  "riemann:Ksy:tKEL:5013056.165892857142857143",
  "subzero:Ksy:tCELL:1100000.0",
  "KelVPN:Ksy:KEL:0.3",
  "Backbone:Ksy:CELL:0.5",
  "riemann:Max:tKEL:21.2",
  "subzero:Max:tCELL:30000.0",
  "KelVPN:Max:KEL:1.95",
  "Backbone:Max:KEL:1.0",
  "Backbone:Max:CELL:1.9",
  "riemann:One:tKEL:99999999988435461.135476090476190476",
  "riemann:One:mtKEL:81.0",
  "subzero:One:tCELL:9697903.29999999999982",
  "subzero:One:mtCELL:0.0",
  "KelVPN:One:KEL:0.5",
  "Backbone:One:CELL:0.6",
  "riemann:Pa:tKEL:0.0",
  "riemann:Seven:tKEL:0.02",
  "subzero:Seven:tCELL:19998.9875",
  "riemann:Six:tKEL:1991.0",
  "riemann:Sofa:tKEL:0.0",
  "subzero:Sofa:tCELL:100000.0",
  "subzero:Sofa:mtCELL:0.0000000000000001",
  "riemann:Vit:tKEL:4008.28",
  "subzero:Vit:tCELL:9989998230.2",
  "KelVPN:Vit:KEL:2.320627615900611274",
  "KelVPN:Vit:mKEL:0.0022",
  "KelVPN:Vit:CELL:2.2",
  "Backbone:Vit:CELL:7.633665106617068435",
  "Backbone:Vit:KEL:97.9",
  "Backbone:Vit:mCELL:0.0176",
  "riemann:Vita:tKEL:29971.603361002604166668",
  "KelVPN:Vita:KEL:1.0",
  "KelVPN:Vita:CELL:1.0",
  "Backbone:Vita:CELL:5.149764009000622262",
  "Backbone:Vita:mCELL:0.0",
  "Backbone:Vita:KEL:1.0",
  "riemann:blade:tKEL:5149335.840975864955357142",
  "KelVPN:blade:KEL:15.778369431037062792",
  "KelVPN:blade:mKEL:0.0062",
  "KelVPN:blade:CELL:0.6",
  "Backbone:blade:KEL:4.270177074250188836",
  "Backbone:blade:CELL:2.045324578770658747",
  "Backbone:blade:mCELL:0.0057",
  "Backbone:blade:mKEL:0.0011",
  "riemann:maka:tKEL:10500.0",
  "KelVPN:maka:KEL:15.4",
  "KelVPN:maka:CELL:0.1",
  "Backbone:maka:KEL:7.0",
  "Backbone:maka:CELL:0.1",
};

#endif // SIMULATE_DATA

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

#ifdef SIMULATE_DATA
  /* variables */

  quint32 i = 0;

  /* cycle thru networks */

  for (i = 0; i < ARRAYSIZE (s_simNetworks); i++)
    _data.networkList << s_simNetworks[i];

  /* cycle thru wallets */

  for (i = 0; i < ARRAYSIZE (s_simWallets); i++)
  {
    QStringList walletFields  = QString (s_simWallets[i]).split (':');

    if (walletFields.size() != 3)
      continue;

    _data.walletTokenList <<
    DapNodeWalletDataStruct::WalletToken
    {
      walletFields.at(0),
      walletFields.at(1),
      walletFields.at(2)
    };
  }

  /* cycle thru tokens */

  for (i = 0; i < ARRAYSIZE (s_simTokens); i++)
  {
    QStringList tokenFields  = QString (s_simTokens[i]).split (':');

    if (tokenFields.size() != 4)
      continue;

    _data.tokenBalanceList <<
    DapNodeWalletDataStruct::TokenBalance
    {
      tokenFields.at(0),
      tokenFields.at(1),
      tokenFields.at(2),
      tokenFields.at(3)
    };
  }
#endif // SIMULATE_DATA

#ifdef PRINT_WALLET_DATA
  _printWalletsData();
#endif // PRINT_WALLET_DATA
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

#ifndef SIMULATE_DATA
      /* parse result data */
      _parseWalletNetworkTokenData (wallet);
#endif // SIMULATE_DATA

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

void DapNodeWalletData::_printWalletsData()
{
  DEBUGINFO << "networks";

  for (const auto &network : qAsConst (_data.networkList))
    qDebug() << network;

  DEBUGINFO << "wallets";

  for (const auto &wallet : qAsConst (_data.walletTokenList))
    qDebug() << QString ("%1:%2:%3").arg (wallet.network, wallet.wallet, wallet.token);

  DEBUGINFO << "tokens";

  for (const auto &token : qAsConst (_data.tokenBalanceList))
    qDebug() << QString ("%1:%2:%3:%4").arg (token.network, token.wallet, token.token, token.balance);
}

/*-----------------------------------------*/
