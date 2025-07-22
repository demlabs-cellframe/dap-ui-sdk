#include "DapNetworkAccessManager.h"
#include "DapHttpPing.h"

DapNetworkAccessManager::DapNetworkAccessManager()
                    :QObject(nullptr)
{

}

void DapNetworkAccessManager::requestHttp_POST(const QString &address, const uint16_t port, const QString &urlPath, const QByteArray &body, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - POST: " << urlPath ;
    bRunning = true;
    dap_client_http_set_params(20000, 5000, 1024*1024);
    dap_client_http_request(nullptr, qPrintable(address), port, "POST", "text/plain", qPrintable(urlPath), body.constData(), static_cast<size_t>(body.size()), nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError,
                                   &netReply, headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::requestHttp_GET(const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - GET: " << urlPath ;
    bRunning = true;
    dap_client_http_set_params(20000, 5000, 1024*1024);
    dap_client_http_request(nullptr, qPrintable(address), port, "GET", "text/plain", qPrintable(urlPath), nullptr, 0, nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError, &netReply,
                                   headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::requestHttp_GET_long_timeout(const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - GET (long timeout): " << urlPath ;
    bRunning = true;
    dap_client_http_set_params(20000, 5000, 1024*1024);
    dap_client_http_set_read_timeout_ms(60000); // 60 seconds for long operations
    dap_client_http_request(nullptr, qPrintable(address), port, "GET", "text/plain", qPrintable(urlPath), nullptr, 0, nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError, &netReply,
                                   headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::requestHttp_GET_for_ping(const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "[DapNetworkAccessManager] Dap Client HTTP Requested - GET: " << urlPath ;
    bRunning = true;
    dap_client_http_request(nullptr, qPrintable(address), port, "GET", "text/plain", qPrintable(urlPath), nullptr, 0, nullptr,
                            &DapHttpPing::responseCallback, &DapHttpPing::responseCallbackError, &netReply,
                            headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::responseCallback(void * a_response, size_t a_response_size, void * a_obj, http_status_code_t a_status_code)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    reply->setReply(QByteArray(reinterpret_cast<const char*>(a_response), static_cast<int>(a_response_size)));
    qDebug() << "[DapNetworkAccessManager]Dap Client HTTP Request: response received, size=" << a_response_size;
    
    // Reset read timeout to default value (5 seconds) after any request
    dap_client_http_set_read_timeout_ms(5000);
    
    reply->setError( 0 );
    emit reply->finished();
    reply->deleteLater();
}

void DapNetworkAccessManager::responseCallbackError(int a_err_code, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    reply->setError(a_err_code);
    char buf[400] = { };
    strerror_r(a_err_code, buf, sizeof(buf));
    qWarning() << "[DapNetworkAccessManager]Dap Client HTTP Request: error code " << a_err_code
               << ": " << buf;
    reply->setErrorStr(buf);
    
    // Reset read timeout to default value (5 seconds) after any request (including errors)
    dap_client_http_set_read_timeout_ms(5000);
    
/*#else
               ;
    {
        char buf[400] = { };
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL,
                      a_err_code,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      buf,
                      sizeof(buf),
                      NULL);
        reply->setErrorStr(buf);
        qWarning() << reply->errorString();
    }
#endif*/
    emit reply->sigError();
    reply->deleteLater();
}

void DapNetworkAccessManager::responseProgressCallback(size_t a_response_size, size_t a_content_length, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    qDebug() << "[DapNetworkAccessManager]Dap Client HTTP Progress update: response received, size=" << a_response_size;
    reply->setContentLength(a_content_length);
    reply->setResponseSize(a_response_size);
    emit reply->progressUpdate(a_response_size);
}
