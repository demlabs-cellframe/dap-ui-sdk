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
  //"subzero",
  "KelVPN",
  "Backbone",
};

static const char* const s_simWallets[] =
{
  "KelVPN:Jax:KEL",
  "KelVPN:Jax:CELL",
  "riemann:Jax:tKEL",
  "Backbone:Jax:CELL",
  "Backbone:Jax:mCELL",
  "Backbone:Jax:KEL",
  "Backbone:Jax:mKEL",
  "KelVPN:Ksu:KEL",
  "KelVPN:Ksu:CELL",
  "KelVPN:Ksu:mKEL",
  "riemann:Ksu:tKEL",
  "riemann:Ksu:mtKEL",
  "Backbone:Ksu:CELL",
  "Backbone:Ksu:mCELL",
  "Backbone:Ksu:KEL",
  "Backbone:Ksu:mKEL",
  "KelVPN:Vit:KEL",
  "KelVPN:Vit:mKEL",
  "KelVPN:Vit:CELL",
  "riemann:Vit:tKEL",
  "Backbone:Vit:CELL",
  "Backbone:Vit:KEL",
  "Backbone:Vit:mCELL",
  "KelVPN:blade:KEL",
  "KelVPN:blade:mKEL",
  "KelVPN:blade:CELL",
  "riemann:blade:tKEL",
  "Backbone:blade:KEL",
  "Backbone:blade:CELL",
  "Backbone:blade:mCELL",
  "Backbone:blade:mKEL",
  "Backbone:blade:USDC",
  "KelVPN:maka:KEL",
  "KelVPN:maka:CELL",
  "KelVPN:maka:mKEL",
  "riemann:maka:tKEL",
  "Backbone:maka:KEL",
  "Backbone:maka:CELL",
};

static const char* const s_simTokens[] =
{
  "KelVPN:Jax:KEL:1.839999999999999981",
  "KelVPN:Jax:CELL:0.100032773666958088",
  "riemann:Jax:tKEL:2000.5",
  "Backbone:Jax:CELL:3.579321293069036859",
  "Backbone:Jax:mCELL:0.00491",
  "Backbone:Jax:KEL:0.6",
  "Backbone:Jax:mKEL:0.0002",
  "KelVPN:Ksu:KEL:8.204687713197754667",
  "KelVPN:Ksu:CELL:1.000000000000000001",
  "KelVPN:Ksu:mKEL:0.0014",
  "riemann:Ksu:tKEL:253122.459642857142857122",
  "riemann:Ksu:mtKEL:0.2",
  "Backbone:Ksu:CELL:22.922954227860886337",
  "Backbone:Ksu:mCELL:0.002096011780276446",
  "Backbone:Ksu:KEL:745.998885233311252703",
  "Backbone:Ksu:mKEL:0.09",
  "KelVPN:Vit:KEL:2.270907743240728248",
  "KelVPN:Vit:mKEL:0.0018",
  "KelVPN:Vit:CELL:2.000084477047285339",
  "riemann:Vit:tKEL:1257.917499999999999998",
  "Backbone:Vit:CELL:10.954120263217176912",
  "Backbone:Vit:KEL:96.5",
  "Backbone:Vit:mCELL:0.0125",
  "KelVPN:blade:KEL:64.940055489487397211",
  "KelVPN:blade:mKEL:0.02705",
  "KelVPN:blade:CELL:3.700088580228164523",
  "riemann:blade:tKEL:5059725.625975864955357142",
  "Backbone:blade:KEL:9.170177074250188836",
  "Backbone:blade:CELL:2.482206053098364864",
  "Backbone:blade:mCELL:0.0031",
  "Backbone:blade:mKEL:0.0013",
  "Backbone:blade:USDC:0.2",
  "KelVPN:maka:KEL:0.3",
  "KelVPN:maka:CELL:0.980060189048711399",
  "KelVPN:maka:mKEL:0.0002",
  "riemann:maka:tKEL:0.0",
  "Backbone:maka:KEL:6.0",
  "Backbone:maka:CELL:0.000118112708224105",
};

static const char* const s_simNetworkFee[] =
{
  "riemann:tKEL",
  "subzero:tCELL",
  "Backbone:CELL",
  "KelVPN:KEL",
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

  connect (this, &DapNodeWalletData::sigDataUpdated,
           this, [this] { _cleanInvalidTokens(); });
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
  //DEBUGINFO << a_walletsData;
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

  /* send networks */
  _emitNetworksList();

  /* update result data */
  emit sigDataUpdated();

#ifdef PRINT_WALLET_DATA
  _printWalletsData();
#endif // PRINT_WALLET_DATA
}

void DapNodeWalletData::setOrderListData (const QJsonArray &a_ordesListData)
{
}

void DapNodeWalletData::setNetworkFee (const QString &a_networkName, const QString &a_fee, const QString &a_feeValue)
{
#ifndef SIMULATE_DATA
  /*-----------------------------------------*/

  for (auto wallet = _wallets.begin(); wallet != _wallets.end(); wallet++)
    for (auto network = wallet->networks.begin(); network != wallet->networks.end(); network++)
      if (network->name == a_networkName)
      {
        network->feeTicker  = a_fee;
        network->feeValue   = a_feeValue;

        /* store fee data */
        _data.networkFeeMap.insert(
          a_networkName,
          DapNodeWalletDataStruct::NetworkFee {
              a_networkName,
              a_fee,
              a_feeValue
            });
      }

  /*-----------------------------------------*/
#else // SIMULATE_DATA
  /*-----------------------------------------*/

  typedef QPair<QString,QString> SimPair;

  QList<SimPair> simData;
  QStringList simSplit;

  /* collect sim data*/
  for (quint32 i = 0; i < ARRAYSIZE (s_simNetworkFee); i++)
  {
    simSplit = QString (s_simNetworkFee[i]).split(':');
    simData << SimPair { simSplit.at(0), simSplit.at(1) };
  }

  for (auto wallet = _wallets.begin(); wallet != _wallets.end(); wallet++)
    for (auto network = wallet->networks.begin(); network != wallet->networks.end(); network++)
      for (const SimPair &pair : qAsConst (simData))
        if (network->name == pair.first)
          network->feeTicker  = pair.second;

  /*-----------------------------------------*/
#endif // SIMULATE_DATA
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

const QMap<QString, DapNodeWalletDataStruct::NetworkFee> &DapNodeWalletData::networkFeeMap() const
{
  return _data.networkFeeMap;
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
  _data.tokenBalanceList.clear();
  _data.walletTokenList.clear();
  _data.networkFeeMap.clear();
  _data.networkList.clear();
//  _fast.walletNames.clear();

  for (auto i = a_data.constBegin(), e = a_data.constEnd(); i != e; i++)
    {
      Wallet wallet;

      /* fill with data */
      wallet.name = i.key();
      //DEBUGINFO << wallet.name;
      _parseNetworks (i.value().toArray(), wallet);

#ifndef SIMULATE_DATA
      /* parse result data */
      _parseWalletNetworkTokenData (wallet);
#endif // SIMULATE_DATA

      /* store name and data */
//    _fast.walletNames.append (wallet.name);
      _wallets.append (std::move (wallet));
    }
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
      //DEBUGINFO << network.name;
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
      //DEBUGINFO << token.name;

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
//        /* magic token name */
//        QString tokenName = QString ("%1 %2").arg (token.balance, token.name);

        /* store new WalletToken */
        _data.walletTokenList
            << DapNodeWalletDataStruct::WalletToken { a_network.name, a_wallet.name, token.name, token.balance };

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

void DapNodeWalletData::_emitNetworksList()
{
//  /* variables */
//  QStringList result;

//  /* collect network list */
//  for (auto wallet = _wallets.begin(); wallet != _wallets.end(); wallet++)
//    for (auto network = wallet->networks.begin(); network != wallet->networks.end(); network++)
//      if (!result.contains (network->name))
//        result.append (network->name);

//  /* send result */
//  emit sigNetworksList (result);

  emit sigNetworksList (_data.networkList);
}

void DapNodeWalletData::_cleanInvalidTokens()
{
  /* map contains network:wallet to token relations */
  QHash<QString, QString> networkWalletTokens;

  /* lambda's */
  auto nwKeyString = [] (const QString &a_network, const QString &a_wallet) -> QString
  {
    return QString ("%1:%2").arg (a_network, a_wallet);
  };

  auto nwKeyStruct = [] (const Network &a_network, const Wallet &a_wallet) -> QString
  {
    return QString ("%1:%2").arg (a_network.name, a_wallet.name);
  };

#ifndef SIMULATE_DATA
  /*-----------------------------------------*/

  /* fill map */
  for (auto wallet = _wallets.begin(); wallet != _wallets.end(); wallet++)
    for (auto network = wallet->networks.begin(); network != wallet->networks.end(); network++)
      if (!network->feeTicker.isEmpty())
        networkWalletTokens.insert (nwKeyStruct (*network, *wallet), network->feeTicker);

  /*-----------------------------------------*/
#else // SIMULATE_DATA
  /*-----------------------------------------*/

  typedef QPair<QString,QString> SimPair;

  QList<SimPair> simData;
  QStringList simSplit;

  /* collect sim data*/
  for (quint32 i = 0; i < ARRAYSIZE (s_simNetworkFee); i++)
  {
    simSplit = QString (s_simNetworkFee[i]).split(':');
    simData << SimPair { simSplit.at(0), simSplit.at(1) };
  }

  for (auto wt = _data.walletTokenList.begin(); wt != _data.walletTokenList.end(); wt++)
    for (const SimPair &pair : qAsConst (simData))
      if (wt->network == pair.first)
        networkWalletTokens.insert (nwKeyString (wt->network, wt->wallet), pair.second);

  /*-----------------------------------------*/
#endif // SIMULATE_DATA

  /* check */
  if (networkWalletTokens.isEmpty())
    return;

  /* clean wallet-tokens */
  for (auto wt = _data.walletTokenList.begin(); wt != _data.walletTokenList.end(); wt++)
  {
//_repeatCleaning:
    /* get proper token */
    QString token = networkWalletTokens.value (nwKeyString (wt->network, wt->wallet));

    /* skip if empty */
    if (token.isEmpty())
      continue;

    /* remove wallet with wrong token */
    if (wt->token != token)
    {
      wt  = _data.walletTokenList.erase (wt);
      wt--;
    }
  }

  /* Step 2: Add wallets with zero balance only if they have no native tokens */
  
  // Create a map to track which wallet-network combinations have native tokens
  QMap<QString, bool> walletHasNativeToken;
  
  // First, mark all existing wallet-network combinations as having tokens
  for (const auto &wt : _data.walletTokenList) {
    QString key = nwKeyString (wt.network, wt.wallet);
    walletHasNativeToken[key] = true;
  }
  
  // Check each wallet-network combination from the original wallets
  for (auto wallet = _wallets.begin(); wallet != _wallets.end(); wallet++) {
    for (auto network = wallet->networks.begin(); network != wallet->networks.end(); network++) {
      if (!network->feeTicker.isEmpty()) {
        QString key = nwKeyString (network->name, wallet->name);
        QString nativeToken = network->feeTicker;
        
        // Check if this wallet-network combination already has any tokens
        if (!walletHasNativeToken.contains (key)) {
          // Add wallet with native token and zero balance
          _data.walletTokenList << DapNodeWalletDataStruct::WalletToken {
            network->name,    // network
            wallet->name,     // wallet
            nativeToken,      // token (native)
            "0"               // balance (zero)
          };
          
          // Also add to token balance list
          _data.tokenBalanceList << DapNodeWalletDataStruct::TokenBalance {
            network->name,    // network
            wallet->name,     // wallet
            nativeToken,      // token (native)
            "0"               // balance (zero)
          };
        }
      }
    }
  }
}

/*-----------------------------------------*/
