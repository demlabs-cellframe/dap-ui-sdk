#include "DapHttpPing.h"
#include "DapConnectClient.h"
#include <QElapsedTimer>

DapHttpPingNetworkReply* DapHttpPing::sendRequest(const QString& host, quint16 port)
{
    auto networkReply = DapConnectClient::instance()->request_GET(host,
                                                                  port, // temporary url For dapServer
                                                                  "/server-info/version");
    return new DapHttpPingNetworkReply(networkReply, host, port);
}
