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
    void sendWalletsList(QStringList walletsList);
    void sendNetworksList(QStringList walletsList);
    void sendError(int code, QString errorMessage);
    void sendNodeDetected();
    void setNodeDetected();
signals:
    void startNodeDetection();
    void stopNodeDetection();
    void dataWalletRequest();
    void condTxCreateRequest(QString tokenName, QString walletName, QString certName, qreal value);
};

#endif // DAPCMDNODEHANDLER_H
