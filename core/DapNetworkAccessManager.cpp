#include "DapNetworkAccessManager.h"

DapNetworkAccessManager::DapNetworkAccessManager()
                    :QObject(nullptr)
{

}

void DapNetworkAccessManager::requestHttp_POST(const QString &address, const uint16_t port, const QString &urlPath, const QByteArray &body, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - POST: " << urlPath ;
    bRunning = true;
    dap_client_http_request_custom(nullptr, qPrintable(address), port, "POST", "text/plain", qPrintable(urlPath), body.constData(), static_cast<size_t>(body.size()), nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError, &netReply, headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::requestHttp_GET(const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - GET: " << urlPath ;
    bRunning = true;
    dap_client_http_request_custom(nullptr, qPrintable(address), port, "GET", "text/plain", qPrintable(urlPath), nullptr, 0, nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError, &netReply, headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::responseCallback(void * a_response, size_t a_response_size, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
//    manager->bRunning = false;
    reply->setReply(QByteArray(reinterpret_cast<const char*>(a_response), static_cast<int>(a_response_size)));
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
    emit reply->finished();
}
