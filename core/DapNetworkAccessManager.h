#ifndef DAPNETWORKACCESSMANAGER_H
#define DAPNETWORKACCESSMANAGER_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QByteArray>

#include "dap_client_http.h"

class DapNetworkAccessManager : public QObject
{
    Q_OBJECT
public:
    DapNetworkAccessManager();
    void requestHttp(const QString &address, const uint16_t &port, const QString & urlPath, const QByteArray & body);
    static void responseCallback(void * a_response, size_t a_response_size, void * a_obj);
    static void responseCallbackError(int a_err_code, void * a_obj);

    bool isRunning(){ return bRunning; };
    QString errorString(){ return error; };
    QString getReplyData(){return reply;};

protected:
    bool bRunning;
    QString error;
    QString reply;

signals:
    void finished();
};

#endif // DAPNETWORKACCESSMANAGER_H
