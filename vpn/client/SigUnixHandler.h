#ifndef SIGUNIXHANDLER_H
#define SIGUNIXHANDLER_H

#include <QObject>

class QSocketNotifier;

class SigUnixHandler : public QObject
{
    Q_OBJECT
    SigUnixHandler(QObject* obj = Q_NULLPTR);
    static void sig_handler_func(int sig_code);
    SigUnixHandler(const SigUnixHandler &) = delete;
public:
    static SigUnixHandler * getInstance() { static SigUnixHandler handler_obj; return &handler_obj; }
signals:
    void sigKill();
private slots:
    void handleSignal();
private:
    static int s_sigFd[2];
    QSocketNotifier *m_notifier = nullptr;
};

#endif // SIGUNIXHANDLER_H
