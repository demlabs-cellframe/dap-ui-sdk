#ifndef DAPNETWORKREPLY_H
#define DAPNETWORKREPLY_H

#include <QObject>
#include <QDebug>

class DapNetworkReply: public QObject
{
    Q_OBJECT
public:
    explicit DapNetworkReply();

    enum NetworkError {
        Error = -1,
        NoError = 0,

        // network layer errors [relating to the destination server] (1-99):
        ConnectionRefusedError = 1,
        RemoteHostClosedError,
        HostNotFoundError,
        TimeoutError,
        OperationCanceledError,
        SslHandshakeFailedError,
        TemporaryNetworkFailureError,
        NetworkSessionFailedError,
        BackgroundRequestNotAllowedError,
        TooManyRedirectsError,
        InsecureRedirectError,
        UnknownNetworkError = 99,

        // proxy errors (101-199):
        ProxyConnectionRefusedError = 101,
        ProxyConnectionClosedError,
        ProxyNotFoundError,
        ProxyTimeoutError,
        ProxyAuthenticationRequiredError,
        UnknownProxyError = 199,

        // content errors (201-299):
        ContentAccessDenied = 201,
        ContentOperationNotPermittedError,
        ContentNotFoundError,
        AuthenticationRequiredError,
        ContentReSendError,
        ContentConflictError,
        ContentGoneError,
        UnknownContentError = 299,

        // protocol errors
        ProtocolUnknownError = 301,
        ProtocolInvalidOperationError,
        ProtocolFailure = 399,

        // Server side errors (401-499)
        InternalServerError = 401,
        OperationNotImplementedError,
        ServiceUnavailableError,
        UnknownServerError = 499
    };

    NetworkError error(){ return m_error; };
    QString errorString(){return m_errorString;}
    QString getReplyData(){return m_reply;};
    QByteArray readAll(){return m_reply.toLatin1();};
    void setReply(const QString &a_reply) {m_reply = a_reply;};
    void setError(NetworkError a_error){m_error = a_error;};
    bool isFinished(){return m_finished;}

signals:
    void finished();
    void sigError();

protected:
//    static void responseCallback(void * a_response, size_t a_response_size, void * a_obj);
//    static void responseCallbackError(int a_err_code, void * a_obj);

    NetworkError m_error;
//    QString errorString;
    QString m_reply;
    QString m_errorString;
    bool m_finished;

};

#endif // DAPNETWORKREPLY_H
