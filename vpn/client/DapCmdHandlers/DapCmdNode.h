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
    bool   m_nocdbMode;
    void   sendNoCdbMode();
public:
    explicit DapCmdNode(QObject *parent = nullptr);
    virtual ~DapCmdNode() override;

    void handle(const QJsonObject* params) override;
    bool nocdbMode() { return m_nocdbMode; }
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
    void sendSigningInfo(qint32 utype, qint64 uid, qint64 units, QString price);
signals:
    void startNodeDetection();
    void stopNodeDetection();
    void dataWalletRequest();
    void orderListRequest(QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit);
    void condTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit);
    void checkSigned();
    void connectByOrder(QString srvUid, QString nodeAddress);
};

#endif // DAPCMDNODEHANDLER_H
