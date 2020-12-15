#include "DapNetworkAccessManager.h"

DapNetworkAccessManager::DapNetworkAccessManager()
                    :QObject(nullptr)
{

}

void DapNetworkAccessManager::requestHttp_POST(const QString &address, const uint16_t &port, const QString & urlPath, const QByteArray & body, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - POST: " << urlPath ;
    bRunning = true;
    dap_client_http_request(NULL,
                            address.toLocal8Bit().constData(),
                            port,
                            "POST",
                            "text/text",
                            urlPath.toLocal8Bit().constData(),
                            body.constData(),
                            body.size(),
                            NULL,
                            &DapNetworkAccessManager::responseCallback,
                            &DapNetworkAccessManager::responseCallbackError,
                            &netReply,
                            NULL);
}

void DapNetworkAccessManager::requestHttp_GET(const QString &address, const uint16_t &port, const QString & urlPath, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - GET: " << urlPath ;
    bRunning = true;
    dap_client_http_request(NULL,
                            address.toLocal8Bit().constData(),
                            port,
                            "GET",
                            "text/text",
                            urlPath.toLocal8Bit().constData(),
                            NULL,
                            NULL,
                            NULL,
                            &DapNetworkAccessManager::responseCallback,
                            &DapNetworkAccessManager::responseCallbackError,
                            &netReply,
                            NULL);
}

void DapNetworkAccessManager::responseCallback(void * a_response, size_t a_response_size, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
//    manager->bRunning = false;
    QString *test = new QString (QString::fromLatin1( reinterpret_cast<const char*>(a_response), a_response_size));
    reply->setReply(QString::fromLatin1( reinterpret_cast<const char*>(a_response), a_response_size));

    qDebug() << "Dap Client HTTP Request: response received, size=" << a_response_size;
    reply->setError(DapNetworkReply::DapNetworkError::NoError);
    emit reply->finished();
}

void DapNetworkAccessManager::responseCallbackError(int a_err_code, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
//    manager->bRunning = false;
    reply->setError(DapNetworkReply::DapNetworkError::Error);

    qWarning() << "Dap Client HTTP Request: error code " << a_err_code ;
}
