#ifndef DAPNETWORKREPLY_H
#define DAPNETWORKREPLY_H

#include <QObject>
#include <QDebug>

class DapNetworkReply: public QObject
{
    Q_OBJECT
public:
    explicit DapNetworkReply();

    int error() const { return m_error; } // a native, platform-dependent network error (see errno.h)
    QString errorString(){ return m_errorString; }
    QByteArray &getReplyData(){ return m_reply; }

    void setReply(const QByteArray &a_reply) { m_reply = a_reply; }
    void setError(int a_error) { m_error = a_error; }
    void setErrorStr(QString a_str) { m_errorString = a_str; }
    bool isFinished(){ return m_finished; }
    QByteArray m_reply;

signals:
    void finished();
    void sigError();

protected:
   int m_error;
    QString m_errorString;
    bool m_finished;

};

#endif // DAPNETWORKREPLY_H
