#ifndef DAPSERVERSLISTREQUESTER_H
#define DAPSERVERSLISTREQUESTER_H

#include <QObject>
#include <QNetworkReply>
#include <DapConnectClient.h>
#include "DapServerInfo.h"

class DapServersListNetworkReply : public QObject {
    Q_OBJECT
private:
public:
    explicit DapServersListNetworkReply(QNetworkReply *networkReply);
signals:
    void sigResponse(DapServerInfoList& servers);
    void sigNetworkError(QNetworkReply::NetworkError);
    void sigParseResponseError();
};

/**
 * @brief The DapServersListRequester class
 * @details Response example:
 * [
        {
          "Name" : "ap-de-0-divevpn",
          "Port" : 8002,
          "Address" : "89.163.221.220",
          "Description" : "",
          "Country" : {
             "Name" : "Germany"
          }
        }
   ]
 */
class DapServersListRequester
{
private:
    explicit DapServersListRequester() {}
public:
    static DapServersListNetworkReply* sendRequest(const QString& host, quint16 port = 443) {
        auto networkReply = DapConnectClient::instance()->request_GET(host,
                                                                      port,
                                                                      "/api/servers/list",
                                                                      true);
        return new DapServersListNetworkReply(networkReply);
    }
};


#endif // DAPSERVERSLISTREQUESTER_H
