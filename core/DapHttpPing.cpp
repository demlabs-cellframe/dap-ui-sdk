#include "DapHttpPing.h"
#include <QElapsedTimer>

void DapHttpPing::sendRequest(const QString& host, quint16 port)
{
  DapNetworkReply *networkReply =  new DapNetworkReply;

  QElapsedTimer *timer = new QElapsedTimer;
  timer->start();

  DapConnectClient::instance()->request_GET( host, port, "", *networkReply );

  connect( networkReply, &DapNetworkReply::finished, this, [=] {

    if ( networkReply->error() == QNetworkReply::NetworkError::NoError ) {
      emit sigResponse(timer->elapsed());
    } else {
      emit sigNetworkError(networkReply->errorString());
    }
    delete timer;
  });
}
