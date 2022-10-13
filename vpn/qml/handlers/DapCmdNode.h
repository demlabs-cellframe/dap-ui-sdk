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

    void sendNodeInfoRequest();
    void sendNodeInfoRequestStop();

    bool error() { return m_hasError; }

signals:
    void walletsList(QStringList list);
    void nodeError(int code, QString errorMessage);

private:
    void sendShowInterface(const QString &interfaceName);
};

#endif // DAPCMDNODEHANDLER_H
