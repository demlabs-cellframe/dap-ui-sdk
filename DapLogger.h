#ifndef DAPLOGGER_H
#define DAPLOGGER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include "dap_common.h"

class DapLogger : public QObject
{
    Q_OBJECT
private:

    QFile *m_logFile = Q_NULLPTR;
    static void messageHandler(QtMsgType type, const QMessageLogContext &ctx,
                               const QString & msg);
    inline static log_level castQtMsgToDap(QtMsgType type);
public:
    explicit DapLogger(QObject *parent = nullptr);

    // return false if not success
    bool setLogFile(const QString& filePath);
};

#endif // DAPLOGGER_H
