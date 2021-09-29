#ifndef DAPNETWORKREPLY_H
#define DAPNETWORKREPLY_H

#include <QObject>
#include <QDebug>

class DapNetworkReply: public QObject
{
    Q_OBJECT
public:
    explicit DapNetworkReply();

    enum DapNetworkError {
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
    Q_ENUM(DapNetworkError)

    DapNetworkReply::DapNetworkError error() const;
    int errorNative() const;

    QString errorString(){ return m_errorString; }
    QByteArray &getReplyData(){ return m_reply; }

    void setReply(const QByteArray &a_reply) { m_reply = a_reply; }
    void setError(DapNetworkError a_error) { m_error = a_error; }
    void setErrorNative(int a_err) { m_errorNative = a_err; }
    bool isFinished(){ return m_finished; }
    QByteArray m_reply;

signals:
    void finished();
    void sigError();

protected:
//    static void responseCallback(void * a_response, size_t a_response_size, void * a_obj);
//    static void responseCallbackError(int a_err_code, void * a_obj);
//    QString errorString;

    DapNetworkError m_error;
    int m_errorNative;
    QString m_errorString;
    bool m_finished;

};

#endif // DAPNETWORKREPLY_H
