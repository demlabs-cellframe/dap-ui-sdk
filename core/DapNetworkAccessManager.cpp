#include "DapNetworkAccessManager.h"
#include "DapHttpPing.h"
#include <errno.h>

DapNetworkAccessManager::DapNetworkAccessManager()
                    :QObject(nullptr)
{

}

void DapNetworkAccessManager::requestHttp_POST(const QString &address, const uint16_t port, const QString &urlPath, const QByteArray &body, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - POST: " << urlPath ;
    bRunning = true;
    dap_client_http_set_connect_timeout_ms(20000);
    dap_client_http_request(nullptr, qPrintable(address), port, "POST", "text/plain", qPrintable(urlPath), body.constData(), static_cast<size_t>(body.size()), nullptr,
                            &DapNetworkAccessManager::responseCallback, &DapNetworkAccessManager::responseCallbackError,
                                   &netReply, headers.length() ? const_cast<char*>(qPrintable(headers)) : nullptr);
}

void DapNetworkAccessManager::requestHttp_GET(const QString &address, const uint16_t port, const QString &urlPath, const QString &headers, DapNetworkReply &netReply)
{
    qDebug() << "Dap Client HTTP Requested - GET: " << urlPath ;
    bRunning = true;
    dap_client_http_set_connect_timeout_ms(20000);
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
    reply->setError( 0 );
    emit reply->finished();
    reply->deleteLater();
}

void DapNetworkAccessManager::responseCallbackError(int a_err_code, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    reply->setError(a_err_code);
    QString errorStr = dapClientHttpErrorToString(a_err_code);
    qWarning() << "[DapNetworkAccessManager]Dap Client HTTP Request: error code " << a_err_code
               << ": " << errorStr;
    reply->setErrorStr(errorStr);
    emit reply->sigError();
    reply->deleteLater();
}

QString DapNetworkAccessManager::dapClientHttpErrorToString(int a_err_code)
{
    if(a_err_code == ETIMEDOUT)
        return QStringLiteral("Connection timed out");
#ifdef ECONNREFUSED
    if(a_err_code == ECONNREFUSED)
        return QStringLiteral("Connection refused");
#endif
#ifdef ECONNRESET
    if(a_err_code == ECONNRESET)
        return QStringLiteral("Connection reset by peer");
#endif
#ifdef EHOSTUNREACH
    if(a_err_code == EHOSTUNREACH)
        return QStringLiteral("Host unreachable");
#endif
#ifdef ENETUNREACH
    if(a_err_code == ENETUNREACH)
        return QStringLiteral("Network unreachable");
#endif
    switch(a_err_code)
    {
    case -6:
        return QStringLiteral("Server disconnected prematurely");
    case -7:
        return QStringLiteral("Server replied with headers only");
    case -8:
        return QStringLiteral("Server disconnected without reply");
    default:
        break;
    }
    char buf[400] = {};
    strerror_r(a_err_code, buf, sizeof(buf));
    QString result = QString::fromLocal8Bit(buf);
    if(result.isEmpty() || result.contains("nknown error", Qt::CaseInsensitive))
        return QString("Network error (code %1)").arg(a_err_code);
    return result;
}

void DapNetworkAccessManager::responseProgressCallback(size_t a_response_size, size_t a_content_length, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    qDebug() << "[DapNetworkAccessManager]Dap Client HTTP Progress update: response received, size=" << a_response_size;
    reply->setContentLength(a_content_length);
    reply->setResponseSize(a_response_size);
    emit reply->progressUpdate(a_response_size);
}
