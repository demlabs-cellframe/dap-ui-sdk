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

class DapLogger : public QObject
{
    Q_OBJECT
private:
    static void messageHandler(QtMsgType type, const QMessageLogContext &ctx,
                               const QString & msg);
    inline static dap_log_level castQtMsgToDap(QtMsgType type);
public:
    explicit DapLogger(QObject *parent = nullptr, QString appType = "", size_t prefix_width = 10);

    // return false if not success
    bool setLogFile(const QString& filePath);

    int createLogFolder(QString path);
    void setPermissionFolder(const QString &path);
    void createChangerLogFiles();

    QString getPathToLog(){ return pathToLog; }
    void setPathToLog(QString path){ pathToLog = path; }

    static QString defaultLogPath(const QString a_brand);
    static QString currentLogFileName(const QString a_brand, const QString a_appType);
    static QString currentLogFilePath(const QString a_brand, const QString a_appType);

    QString getCurrentLogName(){ return m_currentLogName; }
    void updateCurrentLogName();

    void setAppType(QString type){m_appType = type;}
    void clearOldLogs();
    static void setLogLevel(dap_log_level ll);
private:
    QTimer t;
    QString pathToLog;
    QString m_currentLogName;
    QString m_appType;

    QFileSystemWatcher * m_watcher;

signals:
    void logFileChanged();

public slots:
    void fileChanged(const QString& path);
    void dirChanged(const QString& path);
};

#endif // DAPLOGGER_H
