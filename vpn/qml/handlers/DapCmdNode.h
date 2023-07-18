/****************************************************************************
**
** This file is part of the DapVpn project.
**
** The class implements the command handler for receiving user data from
** the service.
**
****************************************************************************/

#ifndef DAPCMDNODEHANDLER_H
#define DAPCMDNODEHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "DapCmdClientAbstract.h"

class OrderListData
{
    QJsonArray m_orderListData;
    QString m_unit;

public:
    void setData (const QJsonArray& a_orderListData);
    QMap<QString, QVariant>& orders();

    void setUnit (const QString &unit);

    QStringList order (const QString &hash);
    QJsonObject orderInfo (const QString &hash);

private:
    QMap<QString, QVariant> m_orders;
    void _updateListMap();
};


class WalletsData
{
    QJsonObject m_walletsData;

public:
    void setData(const QJsonObject& a_walletsData)
    {
        m_walletsData = a_walletsData;
    }

    QStringList wallets()
    {
        return  m_walletsData.keys();
    }

    QMap<QString, QVariant> walletsWithTokens()
    {
        QMap<QString, QVariant> walletsWithTokenName;
        foreach (const auto& walletName, m_walletsData.keys())
        {
            QSet<QString> aSet;
            QJsonArray walletData = m_walletsData[walletName].toArray();
            foreach(const auto& item, walletData)
                foreach (const auto& token, item.toObject().value("tokens").toArray())
                    aSet.insert(token.toObject().value("tokenName").toString());
            aSet.remove("0");
            walletsWithTokenName[walletName].setValue(aSet);
        }
        return walletsWithTokenName;
    }

    QStringList networks(const QString& wallet)
    {
        QStringList networks;
        QJsonArray walletData = m_walletsData[wallet].toArray();
        foreach(const auto& item, walletData)
            networks.append(item.toObject().value("network").toString());
        return networks;
    }

    QMap<QString, QVariant> networkWithTokens(const QString& walletName)
    {
        QMap<QString, QVariant> networkWithTokenName;
        QJsonArray walletData = m_walletsData[walletName].toArray();
        foreach(const auto& item, walletData)
        {
            QSet<QString> aSet;
            QString networkName = item.toObject().value("network").toString();
            foreach (const auto& token, item.toObject().value("tokens").toArray())
                aSet.insert(token.toObject().value("tokenName").toString());
            aSet.remove("0");
            networkWithTokenName[networkName].setValue(aSet);
        }
        return networkWithTokenName;
    }

    QMap<QString, QVariant> tokensAmount(const QString& wallet, const QString network)
    {
        QMap<QString, QVariant> tokens;
        QJsonArray walletData = m_walletsData[wallet].toArray();
        foreach(const auto& item, walletData)
            if (network == item.toObject().value("network").toString())
                foreach(const auto& item, item.toObject().value("tokens").toArray())
                    tokens[item.toObject().value("tokenName").toString()]
                            = item.toObject().value("balance").toString();
        if (tokens.contains("0"))
            tokens.remove("0");
        return tokens;
    }

};


class DapCmdNode: public DapCmdClientAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
    bool m_hasError;
    WalletsData  m_dataWallet;
    OrderListData m_orderListData;

public:
    DapCmdNode(QObject *parent = nullptr) :
        DapCmdClientAbstract(DapJsonCmdType::NODE_INFO, parent) {
    }
    /// Virtual destructor.
    virtual ~DapCmdNode() override { }
    /// Process command.
    /// @param params Command parameters.
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

    void startNodeDetection();
    void stopCheckNode();
    void noCdbModeRequest();

    bool error() { return m_hasError; }
    WalletsData* nodeInfo() { return &m_dataWallet; }

public slots:
    void chooseNetwork(QString network);
    void chooseWallet(QString wallet);
    void chooseToken(QString token);
    void setValue(QString value);
    void setUnit(QString unit);
    void chooseOrder(QString hash);
    void condTxCreate();
    void startSearchOrders();
    void setMaxValueUnit(QString price);
    void setMinValueUnit(QString price);
    QStringList orderData(QString hash);
    void checkSigned();
    void startConnectByOrder();

signals:
    void nodeDetected();
    void walletsList(QMap<QString, QVariant> data);
    void networksList(QMap<QString, QVariant> data);
    void tokensInfo(QMap<QString, QVariant> data);
    void orderList(QMap<QString, QVariant> data);
    void tokenAmount(QString token, QString amount);
    void nodeError(int code, QString errorMessage);
    void transactionHashInMempool();
    void transactionHashInledger();
//    void dataWallets(WalletsData* walletsData);
    void continueEnable(bool);
    void signingReceived(qint32 utype, qint64 uid, QString units, QString value);

private:
    void sendShowInterface(const QString &interfaceName);
    bool checkContinue();
    QString m_selectedWalletName;
    QString m_selectedNetworkName;
    QString m_selectedTokenName;
    QString m_value;
    QString m_unit;
    QString m_maxPrice;
    QString m_minPrice;
    QString m_orderHash;
};

#endif // DAPCMDNODEHANDLER_H
