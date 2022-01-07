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
    switch(code) {
    case -32003:
    case -32001:
        emit sigEmptyList(message);
        break;
    case 101:
        emit sigErrorNetwork(tr("Internet is not available. \nError code: ") + QString::number(code) + "\nСheck the connection");
        break;
    case -666:
        emit sigErrorNetwork(tr("Remote server disconnected before he sends all... \nError code: ") + QString::number(code) + "\nСheck the connection");
        break;
    case -667:
        emit sigErrorNetwork(tr("Remote server replied only with headers... Error code: ") + QString::number(code));
        break;
    case -668:
        emit sigErrorNetwork(tr("Remote server disconnected without reply. Try again... \nError code: ") + QString::number(code) + "\nСheck the connection");
        break;
    default:
        emit sigErrorNetwork(message);
        break;
    }
}
