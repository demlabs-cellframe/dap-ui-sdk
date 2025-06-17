#ifndef DAPNETWORKACCESSMANAGER_H
#define DAPNETWORKACCESSMANAGER_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QByteArray>

#include "DapNetworkReply.h"
#include "http_status_code.h"

class DapNetworkAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit DapNetworkAccessManager();
    void requestHttp_POST   (const QString &address, const uint16_t port, const QString &urlPath, const QByteArray &body, const QString &headers, DapNetworkReply &netReply);
    void requestHttp_GET    (const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply);
    void requestHttp_GET_for_ping(const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply);

    bool isRunning(){ return bRunning; }
signals:
    void finished();

protected:
    bool bRunning;
    static void responseCallback(void * a_response, size_t a_response_size, void * a_obj, http_status_code a_status_code);
    static void responseCallbackError(int a_err_code, void * a_obj);
    static void responseProgressCallback(size_t a_response_size, size_t a_content_length, void * a_obj);

};

#endif // DAPNETWORKACCESSMANAGER_H
