#ifndef DAPLOGGER_H
#define DAPLOGGER_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QSettings>
#include <QDir>
#include <QTimer>
#include "dap_common.h"
#include "dap_file_utils.h"

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

    int createLogFolder(QString path);
    void createChangerLogFiles();

    QString getPathToLog(){ return pathToLog; }
    void setPathToLog(QString path){ pathToLog = path; }
    QString getCurrentLogName(){ return currentLogName; }
    void updateCurrentLogName(){
        currentLogName = QString("%1%2_%3.log").arg(DAP_BRAND).arg(appType).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy")); }
    void setAppType(QString type){appType = type;}
    void clearOldLogs();
    static void setLogLevel(dap_log_level ll);
private:
    QTimer *t = new QTimer(QCoreApplication::instance());
    QString pathToLog;
    QString currentLogName;
    QString appType;
};

#endif // DAPLOGGER_H
