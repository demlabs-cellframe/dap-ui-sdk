#include "SigUnixHandler.h"
#include <QDebug>
#include <QCoreApplication>
#include <QSocketNotifier>
#include <csignal>
#include <sys/socket.h>
#include <unistd.h>

int SigUnixHandler::s_sigFd[2] = {-1, -1};

// Async-signal-safe: only writes a byte to the pipe
void SigUnixHandler::sig_handler_func(int sig_code)
{
    char code = static_cast<char>(sig_code);
    ::write(s_sigFd[0], &code, sizeof(code));
}

void SigUnixHandler::handleSignal()
{
    m_notifier->setEnabled(false);

    char code;
    ::read(s_sigFd[1], &code, sizeof(code));
    int sig = static_cast<int>(code);

    qDebug() << "SigUnixHandler get signal:" << sig;
    emit sigKill();
    QCoreApplication::quit();

    m_notifier->setEnabled(true);
}

SigUnixHandler::SigUnixHandler(QObject* obj) : QObject(obj)
{
    qDebug() << "Create Sig Unix Handler object";

    if(::socketpair(AF_UNIX, SOCK_STREAM, 0, s_sigFd))
    {
        qCritical() << "Failed to create socketpair for signal handling";
        return;
    }

    m_notifier = new QSocketNotifier(s_sigFd[1], QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated, this, &SigUnixHandler::handleSignal);

#ifdef Q_OS_LINUX
    ::signal(SIGKILL, sig_handler_func);
#endif
    ::signal(SIGINT, sig_handler_func);
    ::signal(SIGTERM, sig_handler_func);
}
