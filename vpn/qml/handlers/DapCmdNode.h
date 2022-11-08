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

#include "DapCmdServiceAbstract.h"

class DapCmdNode: public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
    bool m_hasError;
public:
    DapCmdNode(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::NODE_INFO, parent) {
    }
    /// Virtual destructor.
    virtual ~DapCmdNode() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;

    void startNodeDetection();
    void stopCheckNode();
    void condTxCreate(QString tokenName, QString walletName, QString certName, qreal value);

    bool error() { return m_hasError; }

signals:
    void nodeDetected();
    void walletsList(QStringList list);
    void networksList(QStringList list);
    void nodeError(int code, QString errorMessage);

private:
    void sendShowInterface(const QString &interfaceName);
    void sendRequest(const QJsonObject &data);
};

#endif // DAPCMDNODEHANDLER_H
