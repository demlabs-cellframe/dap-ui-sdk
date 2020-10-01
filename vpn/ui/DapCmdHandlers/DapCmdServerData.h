/****************************************************************************
**
** This file is part of the DapVpn project.
**
** The class implements the command handler for receiving user data from
** the service.
**
****************************************************************************/

#ifndef DAPCMDSERVERDATAHANDLER_H
#define DAPCMDSERVERDATAHANDLER_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdServiceAbstract.h"
#include "DapServerInfo.h"

class DapCmdServerData: public DapCmdServiceAbstract
{
    Q_OBJECT

    QString     m_address;

    quint16 m_port;
public:
    const QString address() { return  m_address; }
    quint16 port() {return m_port; }
    /// Standart constructor.
    /// @param parent Parent.
    DapCmdServerData(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::SEND_SERVER_DATA, parent) { }
    /// Virtual destructor.
    virtual ~DapCmdServerData() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;

    /// @param address Server address.
    void sendServerData(const QString &address, quint16 a_port);

signals:
    void sentCurrentServerName(const DapServerInfo &server);
};

#endif // DAPCMDSERVERDATAHANDLER_H
