#ifndef DAPLOGGER_H
#define DAPLOGGER_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QFileSystemWatcher>

#include "dap_common.h"
#include "dap_file_utils.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#endif

class DapLogger : public QObject
{
    Q_OBJECT
private:
    static void messageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString & msg);
    inline static dap_log_level castQtMsgToDap(QtMsgType type);
    void setPathToLog(QString path){
      qDebug() << path;
      m_pathToLog = path; }
    void updateCurrentLogName();
    void setLogFile(const QString&);
    void updateLogFiles();
    static void writeMessage(QtMsgType type, const QMessageLogContext &ctx, const QString & msg);
public:
    explicit DapLogger(QObject *parent = nullptr, QString appType = "", size_t prefix_width = 10);
    static DapLogger* instance();
    QString getPathToLog(){ return m_pathToLog; }
    static QString defaultLogPath(const QString a_brand);
    static QString currentLogFileName(const QString a_brand, const QString a_appType);
    static QString currentLogFilePath(const QString a_brand, const QString a_appType);
    QString getCurrentLogName(){ return m_currentLogName; }
    void setAppType(QString type){m_appType = type;}
    void clearOldLogs();
    static void setLogLevel(dap_log_level ll);
private:
    QString m_pathToLog;
    QString m_currentLogName;
    QString m_appType;
    QString m_day;
    QString systemInfo();
signals:
    void sigMessageHandler();
};

#endif // DAPLOGGER_H
