#ifndef DAPSERVERSLISTREQUESTER_H
#define DAPSERVERSLISTREQUESTER_H

#include <QObject>
#include <QNetworkReply>
#include <DapConnectClient.h>
#include "DapServerInfo.h"
#include "DapReplyTimeout.h"

class DapServersListNetworkReply : public QObject {
    Q_OBJECT
private:
public:
    explicit DapServersListNetworkReply(QNetworkReply *networkReply);
    QString errorString() const {
        QNetworkReply * reply = qobject_cast<QNetworkReply *>(parent());
        if(reply == Q_NULLPTR) {
            qCritical() << "Error cast";
            return "";
        }
        return reply->errorString();
    }
signals:
    void sigResponse(const QJsonDocument& doc);
    void sigNetworkError(QNetworkReply::NetworkError);
    void sigParseResponseError();
};

/**
 * @brief The DapServersListRequester class
 * @details Response example:
 * [
        {
          "Name" : "ap-de-0",
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
    static DapServersListNetworkReply* sendRequest(const QString& host, quint16 port = 80) {
        auto networkReply = DapConnectClient::instance()->request_GET(host,
                                                                      port,
                                                                      "/nodelist",
                                                                      true);
        DapReplyTimeout::set(networkReply, 10000); // 10 sec
        return new DapServersListNetworkReply(networkReply);
    }
};


#endif // DAPSERVERSLISTREQUESTER_H
