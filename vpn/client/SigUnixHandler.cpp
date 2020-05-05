#include "SigUnixHandler.h"
#include <QDebug>
#include <QCoreApplication>
#include <csignal>

void SigUnixHandler::sig_handler_func(int sig_code)
{
    qDebug() << "SigUnixHandler get signal: " << sig_code;
    emit SigUnixHandler::getInstance()->sigKill();
    QCoreApplication::quit();
}

SigUnixHandler::SigUnixHandler(QObject* obj) : QObject(obj)
{
    qDebug() << "Create Sig Unix Handler object";
#ifdef Q_OS_LINUX
    ::signal(SIGKILL, sig_handler_func);
#endif
    ::signal(SIGINT, sig_handler_func);
    ::signal(SIGTERM, sig_handler_func);
}
