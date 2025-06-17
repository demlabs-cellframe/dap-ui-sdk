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
  // void sigNetworkError(int, QString);

public:
  bool bRunning;
  static void responseCallback(void * a_response, size_t a_response_size, void * a_obj, http_status_code a_status_code);
  static void responseCallbackError(int a_err_code, void * a_obj);
  static void responseProgressCallback(size_t a_response_size, size_t a_content_length, void * a_obj);
};

#endif // DAPHTTPPING_H
