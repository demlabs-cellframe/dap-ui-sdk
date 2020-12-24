#include "DapHttpPing.h"
#include "DapConnectClient.h"
#include <QElapsedTimer>

DapHttpPingNetworkReply* DapHttpPing::sendRequest(const QString& host, quint16 port)
{
    DapNetworkReply *networkReply =  new DapNetworkReply;
    DapConnectClient::instance()->request_GET(host, port, "server-info/version", *networkReply);
//    return new DapHttpPingNetworkReply(networkReply, host, port);
}
