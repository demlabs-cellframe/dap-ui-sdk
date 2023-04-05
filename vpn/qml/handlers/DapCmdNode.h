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

public:
    void setData(const QJsonArray& a_orderListData)
    {
        m_orderListData = a_orderListData;
        toListMap();
    }
//    const QList<QMap<QString, QString>>& orders()
    QMap<QString, QVariant>& orders()
    {
        return m_orders;
    }
private:
//    QList<QMap<QString, QString>> m_orders;
//    void toListMap()
//    {
//        foreach(const QJsonValue& item, m_orderListData)
//        {
//            QMap<QString, QString> itemDataAsMap;
//            QJsonObject joItem = item.toObject();
//            foreach(const QString& item_key, joItem.keys())
//                itemDataAsMap[item_key] = joItem[item_key].toString();
//            m_orders.push_back(itemDataAsMap);
//        }
//    }
    QMap<QString, QVariant> m_orders;
    void toListMap()
    {
        foreach(const QJsonValue& item, m_orderListData)
        {
            QJsonObject joItem = item.toObject();
            QString key = joItem["hash"].toString().right(10);
            QStringList list;
            if (joItem.contains("node_location")
//                    && joItem.contains("node_addr")
//                    && !joItem["node_addr"].toString().isEmpty()
               )
            {
                list.push_back(joItem["node_location"].toString());
                list.push_back(joItem["price"].toString());
                m_orders[key].setValue(list);
            }
        }
    }
public:
    QStringList order(QString hash)
    {
        foreach(const QJsonValue& item, m_orderListData)
        {
            QJsonObject joItem = item.toObject();
            QString key = joItem["hash"].toString().right(10);
            if (key == hash)
            {
                QStringList list;
                list.push_back(joItem["node_location"].toString());
                list.push_back(joItem["node_addr"].toString());
                m_orders[key].setValue(list);
                return list;
            }
        }
        return QStringList();
    }
    QJsonObject orderInfo(QString hash)
    {
        foreach(const QJsonValue& item, m_orderListData)
        {
            QJsonObject joItem = item.toObject();
            QString key = joItem["hash"].toString().right(10);
            if (key == hash)
            {
//                == Order 0xD9A5C15D30A42615398AB7D3080FDEBCCD74FA3BB2E191F76EAC994326B45AA9 ==
//                  version:          3
//                  direction:        SERV_DIR_SELL
//                  srv_uid:          0x0000000000000001
//                  price:            0.000000000000000002 (2)
//                  price_unit:       DAY
//                  node_addr:        58C0::CA70::6D11::1DCA
//                  node_location:    Europe - Russia_2_1
//                  tx_cond_hash:     0x0000000000000000000000000000000000000000000000000000000000000000
//                  ext:              0x52025275737369615F325F3100
                return joItem;
            }
        }
        return QJsonObject();
    }
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
