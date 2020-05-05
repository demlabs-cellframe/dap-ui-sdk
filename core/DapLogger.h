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
    static void messageHandler(QtMsgType type, const QMessageLogContext &ctx,
                               const QString & msg);
    inline static dap_log_level castQtMsgToDap(QtMsgType type);
public:
    explicit DapLogger(QObject *parent = nullptr, size_t prefix_width = 10);

    // return false if not success
    bool setLogFile(const QString& filePath);

    static void setLogLevel(dap_log_level ll);
};

#endif // DAPLOGGER_H
