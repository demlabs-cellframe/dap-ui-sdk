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
signals:
    void startNodeDetection();
    void stopNodeDetection();
    void dataWalletRequest();
    void condTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit);
};

#endif // DAPCMDNODEHANDLER_H
