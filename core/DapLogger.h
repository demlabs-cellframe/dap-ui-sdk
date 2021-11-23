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
    static void messageHandler(QtMsgType type, const QMessageLogContext &ctx,
                               const QString & msg);
    inline static dap_log_level castQtMsgToDap(QtMsgType type);

public:
    explicit DapLogger(QObject *parent = nullptr, QString appType = "", size_t prefix_width = 10);

    void setLogFile(const QString& filePath);
    void createChangerLogFiles();

    QString getPathToLog(){ return m_pathToLog; }
    void setPathToLog(QString path){ m_pathToLog = path; }

    static QString defaultLogPath(const QString a_brand);
    static QString currentLogFileName(const QString a_brand, const QString a_appType);
    static QString currentLogFilePath(const QString a_brand, const QString a_appType);

    static QString defaultPluginPath(const QString a_brand);
    void setPathToPlugin(QString path){ m_pathToPlugin = path; }
    QString getPathToPlugin(){ return m_pathToPlugin; }

    QString getCurrentLogName(){ return m_currentLogName; }
    void updateCurrentLogName();

    void setAppType(QString type){m_appType = type;}
    void clearOldLogs();
    static void setLogLevel(dap_log_level ll);
private:
    QTimer t;
    QString m_pathToLog;
    QString m_currentLogName;
    QString m_appType;
    QString m_pathToPlugin;
};

#endif // DAPLOGGER_H
