#ifndef DAPHTTPPING_H
#define DAPHTTPPING_H

#include <QNetworkReply>
#include <QElapsedTimer>

#include "DapConnectClient.h"

class DapServerConnectivityChecker;
enum class DapConnectionMethod : int;

class DapHttpPing : public QObject
{
  Q_OBJECT

private:
  QNetworkReply* m_reply;
  QString m_host;
  quint16 m_port;
  DapServerConnectivityChecker* m_connectivityChecker;
  bool m_useMultiLevelCheck;

public:
  DapHttpPing(const QString& a_host, const quint16 a_port)
             : m_host(a_host), m_port(a_port), m_connectivityChecker(nullptr), m_useMultiLevelCheck(true) {}
  ~DapHttpPing();
  
  void sendRequest(const QString& host, quint16 port);

  QString getHost() { return m_host; }
  quint16 getPort() { return m_port; }
  
  // Multi-level connectivity check configuration
  void setUseMultiLevelCheck(bool enabled) { m_useMultiLevelCheck = enabled; }
  bool isUsingMultiLevelCheck() const { return m_useMultiLevelCheck; }

signals:
  void sigResponse(qint64 requestTime);
  void sigNetworkError(QString);
  // void sigNetworkError(int, QString);

private slots:
  void onConnectivityChecked(const QString& host, quint16 port, bool available, 
                           int responseTime, DapConnectionMethod method);
  void onConnectivityCheckFailed(const QString& host, quint16 port, const QString& errorMessage);

public:
  bool bRunning;
  static void responseCallback(void * a_response, size_t a_response_size, void * a_obj, http_status_code_t statuscode);
  static void responseCallbackError(int a_err_code, void * a_obj);
  static void responseProgressCallback(size_t a_response_size, size_t a_content_length, void * a_obj);
  
private:
  void startTraditionalHttpPing(const QString& host, quint16 port);
};

#endif // DAPHTTPPING_H
