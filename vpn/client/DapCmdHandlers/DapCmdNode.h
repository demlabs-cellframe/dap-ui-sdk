#ifndef DAPCMDNODEHANDLER_H
#define DAPCMDNODEHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdNode : public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
    bool   m_nodeDetected;
public:
    explicit DapCmdNode(QObject *parent = nullptr);
    virtual ~DapCmdNode() override;

    void handle(const QJsonObject* params) override;
public slots:
    void sendWalletsList(const QStringList& walletsList);
    void sendNetworksList(const QStringList& walletsList);
    void sendWalletsData(const QJsonObject& walletsData);
    void sendError(int code, const QString& errorMessage);
    void sendNodeDetected();
    void setNodeDetected();
    void sendTransactionInMempool();
    void sendTransactionInLedger();
    void sendOrderList(const QJsonArray& orderList);
    void sendReceipInfo(qint32 utype, qint64 uid, qint64 units, qint64 value);
signals:
    void startNodeDetection();
    void stopNodeDetection();
    void dataWalletRequest();
    void orderListRequest(QString networkName, QString tokenName, QString unit, QString maxPrice, QString minPrice);
    void condTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit);
    void receiptSigned();
    void connectByOrder(QString netId, QString txCondHash, QString token, QString srvUid, QString address, uint16_t port);
};

#endif // DAPCMDNODEHANDLER_H
