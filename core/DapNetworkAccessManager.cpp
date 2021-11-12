#include "DapNetworkAccessManager.h"

DapNetworkAccessManager::DapNetworkAccessManager()
                    :QObject(nullptr)
{

}

void DapNetworkAccessManager::requestHttp_POST(const QString &address, const uint16_t port, const QString &urlPath, const QByteArray &body, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - POST: " << urlPath ;
    bRunning = true;
    dap_client_http_request(nullptr, qPrintable(address), port, "POST", "text/plain", qPrintable(urlPath), body.constData(), static_cast<size_t>(body.size()), nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError,
                                   &netReply, headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::requestHttp_GET(const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - GET: " << urlPath ;
    bRunning = true;
    dap_client_http_request(nullptr, qPrintable(address), port, "GET", "text/plain", qPrintable(urlPath), nullptr, 0, nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError, &netReply,
                                   headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::responseCallback(void * a_response, size_t a_response_size, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    reply->setReply(QByteArray(reinterpret_cast<const char*>(a_response), static_cast<int>(a_response_size)));
    qDebug() << "Dap Client HTTP Request: response received, size=" << a_response_size;
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
    qWarning() << "Dap Client HTTP Request: error code " << a_err_code
               << ": " << buf;
    reply->setErrorStr(buf);
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
