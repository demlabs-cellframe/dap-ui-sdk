#include "DapCmdServersList.h"

DapCmdServersList::DapCmdServersList(QObject *parent)
    : DapCmdClientAbstract (DapJsonCmdType::GET_SERVERS_LIST, parent)
{

}

void DapCmdServersList::handleResult(const QJsonObject& result)
{
    if(result.value("servers").isArray()) {
        DapServerInfoList servers;
        if (DapServerInfo::fromJSON(result.value("servers").toArray(), servers)) {
            if (servers.isEmpty()) {
                emit sigEmptyList("No servers available on this CDB. Try another...");
            } else {
                emit sigServersListUpdate(servers);
            }
        } else {
            qCritical() << "Error parse response from service";
        }
    } else {
        qCritical() << "Bad response from service";
    }
}


void DapCmdServersList::handleError(int code, const QString& message)
{
    Q_UNUSED(message)
    if (code == -666) {
        emit sigEmptyList(message);
    } else if (code == -32002) {
        emit sigErrorNetwork(message);
    } else if (code == -32003) {
        emit sigNetworkUnreachable(message);
    }
    qWarning() << *m_errorObject;
}
