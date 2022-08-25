#ifndef DAPHTTPPING_H
#define DAPHTTPPING_H

#include <QNetworkReply>
#include <QElapsedTimer>

#include "DapConnectClient.h"

class DapHttpPing : public QObject
{
  Q_OBJECT

private:
  QNetworkReply* m_reply;
  QString m_host;
  quint16 m_port;

public:
  DapHttpPing(const QString& a_host, const quint16 a_port)
             : m_host(a_host), m_port(a_port) {}
  void sendRequest(const QString& host, quint16 port);

  QString getHost() { return m_host; }
  quint16 getPort() { return m_port; }

signals:
  void sigResponse(qint64 requestTime);
  void sigNetworkError(QString);
};

#endif // DAPHTTPPING_H
