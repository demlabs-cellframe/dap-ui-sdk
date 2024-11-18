#include "DapHttpPing.h"
#include <QElapsedTimer>

DapHttpPing::~DapHttpPing()
{
  qDebug() << "~DapHttpPing " << this;
}

void DapHttpPing::sendRequest(const QString& host, quint16 port)
{
  DapNetworkReply *networkReply =  new DapNetworkReply;

  QElapsedTimer *timer = new QElapsedTimer;
  timer->start();
  qDebug() << "DapHttpPing sendRequest \t host: " << host << " port: " << port;
  DapConnectClient::instance()->request_GET_for_ping( host, port, "", *networkReply );
  qDebug() << "DapHttpPing " << this;
  connect( networkReply, &DapNetworkReply::finished, this, [timer, networkReply, this , host, port] {
    qDebug() << "DapHttpPing finished \t host: " << host << " port: " << port << " error: " << networkReply->error();

    if ( networkReply->error() == QNetworkReply::NetworkError::NoError )
    {
      emit sigResponse(timer->elapsed());
    }
    else
    {
      emit sigNetworkError(networkReply->errorString());
    }
    qDebug() << "DapHttpPing finished \t host: " << host << " port: " << port;
    delete timer;
  });

  connect( networkReply, &DapNetworkReply::sigError, this, [timer, networkReply, this , host, port]
          {
      emit sigNetworkError(/*networkReply->error(), */networkReply->errorString());
      qDebug() << "DapHttpPing finished \t host: " << host << " port: " << port;
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
