#include "DapHttpPing.h"
#include "DapServerConnectivityChecker.h"
#include <QElapsedTimer>
#include <QDebug>

DapHttpPing::~DapHttpPing()
{
    if (m_connectivityChecker) {
        m_connectivityChecker->deleteLater();
        m_connectivityChecker = nullptr;
    }
}

void DapHttpPing::sendRequest(const QString& host, quint16 port)
{
    if (m_useMultiLevelCheck) {
        // Use multi-level connectivity check
        if (m_connectivityChecker) {
            m_connectivityChecker->deleteLater();
        }
        
        m_connectivityChecker = new DapServerConnectivityChecker(this);
        
        connect(m_connectivityChecker, &DapServerConnectivityChecker::connectivityChecked,
                this, &DapHttpPing::onConnectivityChecked);
        connect(m_connectivityChecker, &DapServerConnectivityChecker::checkFailed,
                this, &DapHttpPing::onConnectivityCheckFailed);
        
        // Start connectivity check
        m_connectivityChecker->checkServerAvailability(host, port, 3000); // 3 seconds timeout
    } else {
        // Use traditional HTTP ping
        startTraditionalHttpPing(host, port);
    }
}

void DapHttpPing::onConnectivityChecked(const QString& host, quint16 port, bool available, 
                                       int responseTime, DapConnectionMethod method)
{
    Q_UNUSED(host)
    Q_UNUSED(port)
    Q_UNUSED(method)
    
    if (available) {
        emit sigResponse(responseTime);
    } else {
        emit sigNetworkError("Server not available through any connectivity check method");
    }
}

void DapHttpPing::onConnectivityCheckFailed(const QString& host, quint16 port, const QString& errorMessage)
{
    Q_UNUSED(host)
    Q_UNUSED(port)
    
    emit sigNetworkError(errorMessage);
}

void DapHttpPing::startTraditionalHttpPing(const QString& host, quint16 port)
{
    DapNetworkReply *networkReply = new DapNetworkReply;

    QElapsedTimer *timer = new QElapsedTimer;
    timer->start();

    DapConnectClient::instance()->request_GET_for_ping(host, port, "", *networkReply);

    connect(networkReply, &DapNetworkReply::finished, this, [=] {
        if (networkReply->error() == QNetworkReply::NetworkError::NoError) {
            emit sigResponse(timer->elapsed());
        } else {
            emit sigNetworkError(networkReply->errorString());
        }
        delete timer;
    });

    connect(networkReply, &DapNetworkReply::sigError, this, [=] {
        emit sigNetworkError(networkReply->errorString());
        delete timer;
    });
}

void DapHttpPing::responseCallback(void * a_response, size_t a_response_size, void * a_obj, http_status_code_t statuscode)
{
    Q_UNUSED(statuscode)

    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    reply->setReply(QByteArray(reinterpret_cast<const char*>(a_response), static_cast<int>(a_response_size)));
    qDebug() << "[DapHttpPing] Dap Client HTTP Request: response received, size=" << a_response_size;
    reply->setError( 0 );
    emit reply->finished();
    reply->deleteLater();
}

void DapHttpPing::responseCallbackError(int a_err_code, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    reply->setError(a_err_code);
    char buf[400] = { };
    strerror_r(a_err_code, buf, sizeof(buf));
    qWarning() << "[DapHttpPing] Dap Client HTTP Request: error code " << a_err_code
               << ": " << buf;
    reply->setErrorStr(buf);
    emit reply->sigError();
    reply->deleteLater();
}

void DapHttpPing::responseProgressCallback(size_t a_response_size, size_t a_content_length, void * a_obj)
{
    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
    qDebug() << "[DapHttpPing] Dap Client HTTP Progress update: response received, size=" << a_response_size;
    reply->setContentLength(a_content_length);
    reply->setResponseSize(a_response_size);
    emit reply->progressUpdate(a_response_size);
}
