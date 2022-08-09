/****************************************************************************
**
** This file is part of the DapVpn project.
**
** The class implements the command handler for receiving user data from
** the service.
**
****************************************************************************/

#ifndef DAPCMDDATALOCALHANDLER_H
#define DAPCMDDATALOCALHANDLER_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdServiceAbstract.h"

class DapCmdDataLocal: public DapCmdServiceAbstract
{
    Q_OBJECT
    static const QString actionParam;
public:
    DapCmdDataLocal(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::DATA_LOCAL, parent) {
    }
    /// Virtual destructor.
    virtual ~DapCmdDataLocal() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;

public slots:
    void allDataRequest();
    void sendSaveRequest(QMap<QString, QVariant> data);
    void sendRemoveRequest(QStringList keys);

signals:
    void dataReceived();

private:
    void sendShowInterface(const QString &interfaceName);
};

#endif // DAPCMDDATALOCALHANDLER_H
