#include "DapCmdServersList.h"

DapCmdServersList::DapCmdServersList(QObject *parent)
    : DapCmdClientAbstract (DapJsonCmdType::GET_SERVERS_LIST, parent)
{

}

void DapCmdServersList::handleResult(const QJsonObject& result)
{
  qDebug() << "DapCmdServersList::handleResult :" << result;
    if(result.value("servers").isArray()) {
        DapServerInfoList servers;
        if (DapServerInfo::fromJSON(result.value("servers").toArray(), servers)) {
            if (servers.isEmpty()) {
                if (m_noCdbMode) {
                    qDebug() << "[noCDB QML] Empty server list in noCDB mode, skipping CDB fallback";
                    emit sigEmptyList (tr ("Empty server list after filtering"));
                } else {
                    emit sigEmptyList (tr ("No servers available on this CDB. Try another..."));
                }
            } else {
                emit sigServersListUpdate(servers, QDateTime::fromSecsSinceEpoch(result.value("update_time").toString().toLongLong()));
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
        if (m_noCdbMode) {
            qDebug() << "[noCDB QML] Error in noCDB mode:" << code << message;
        }
        emit sigEmptyList (message);
        break;
    case 101:
        emit sigErrorNetwork (tr ("Internet is not available. \nError code: %1\nCheck the connection").arg (QString::number (code)));
        break;
    case -666:
        emit sigErrorNetwork (tr ("Remote server disconnected before he sends all... \nError code: %1\nCheck the connection").arg (QString::number (code)));
        break;
    case -667:
        emit sigErrorNetwork (tr ("Remote server replied only with headers... Error code: %1").arg (QString::number (code)));
        break;
    case -668:
        emit sigErrorNetwork (tr ("Remote server disconnected without reply. Try again... \nError code: %1\nCheck the connection").arg (QString::number (code)));
        break;
    default:
        emit sigErrorNetwork (message);
        break;
    }
}
