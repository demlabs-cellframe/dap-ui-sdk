#ifndef DAPNETWORKREPLY_H
#define DAPNETWORKREPLY_H

#include <QObject>
#include <QDebug>

class DapNetworkReply: public QObject
{
    Q_OBJECT
public:
    explicit DapNetworkReply();
    explicit DapNetworkReply(QObject *parent = nullptr) : QObject(parent) { }

    int error() const { return m_error; } // a native, platform-dependent network error (see errno.h)
    QString errorString(){ return m_errorString; }
    QByteArray &getReplyData(){ return m_reply; }
    size_t contentLength() { return m_content_length; }
    size_t responseSize() { return m_response_size; }

    void setReply(const QByteArray &a_reply) { m_reply = a_reply; }
    void setError(int a_error) { m_error = a_error; }
    void setErrorStr(QString a_str) { m_errorString = a_str; }
    bool isFinished(){ return m_finished; }
    void setContentLength(size_t content_length) { m_content_length = content_length; }
    void setResponseSize(size_t response_size) { m_response_size = response_size; }

signals:
    void finished();
    void sigError();
    void progressUpdate(size_t bytes);

protected:
    QByteArray m_reply;
    int m_error;
    QString m_errorString = "";
    bool m_finished;
    size_t m_response_size;
    size_t m_content_length;

};

#endif // DAPNETWORKREPLY_H
