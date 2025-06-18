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
#include <QRegularExpression>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#endif

enum TypeLogCleaning
{
    TWO_DAYS = 0,
    FULL_FILE_SIZE
};

class DapLogger : public QObject
{
    Q_OBJECT
public:
    explicit DapLogger(QObject *parent = nullptr, QString appType = "", size_t prefix_width = 10, TypeLogCleaning typeClean = TypeLogCleaning::TWO_DAYS);
    ~DapLogger();

    static DapLogger* instance();
    static void deleteLogger();
    static QString defaultLogPath(const QString a_brand);
    static QString currentLogFileName(const QString a_brand, const QString a_appType);
    static QString currentLogFilePath(const QString a_brand, const QString a_appType);
    static void setLogLevel(dap_log_level ll);

    static const QString& getPathToLog() { return m_pathToLog; }
    static const QString& getPathToFile() { return m_pathToFile; }
    QString getCurrentLogName(){ return m_currentLogName; }  
    void setAppType(QString type){m_appType = type;}
    void clearOldLogs();
    TypeLogCleaning getTypeLogCleaning() {return m_typeLogCleaning;}
    int getCurrentIndex() {return m_curentIndex;}
    void startUpdateTimer();
private:
    static void messageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString & msg);
    inline static dap_log_level castQtMsgToDap(QtMsgType type);

    void setPathToLog(QString path);
    void updateCurrentLogName();
    void setLogFile(const QString&);
    void updateLogFiles();

    static void writeMessage(QtMsgType type, const QMessageLogContext &ctx, const QString & msg);

    void updateActualIndex();
    QString getCurrentDate();
private slots:
    void updateLogFilesInfo();

private:
    static QString m_pathToLog;
    static QString m_pathToFile;
    QString m_currentLogName = "";
    QString m_appType = "";
    QString m_day = "";
    QString systemInfo();
    bool isLoggerStarted{false};

    TypeLogCleaning m_typeLogCleaning;
    QTimer *m_timeUpdate = nullptr;
    QString m_currentDate;
    int m_curentIndex = 0;
    int m_timeOut = 10000;
    qint64 m_maxSize =  524288000;/// 500 MByte
    qint64 m_partSize = 52428800; /// 50 MByte

    const QRegularExpression INDEX_EXEPTION = QRegularExpression(R"(\((\d+)\))");
signals:
    void sigMessageHandler();
};
#endif // DAPLOGGER_H
