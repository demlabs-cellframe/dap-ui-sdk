#include <stdio.h>
#include <QStandardPaths>
#include <iostream>
#include "dap_common.h"
#include "dap_file_utils.h"
#include "DapLogger.h"
#include "DapDataLocal.h"
#ifdef Q_OS_WIN
#include "registry.h"
#endif

static DapLogger* m_instance = nullptr;

DapLogger::DapLogger(QObject *parent, QString appType, size_t prefix_width)
    : QObject(parent)
    , m_day(QDateTime::currentDateTime().toString("dd"))
{
    m_instance = this;
    dap_set_log_tag_width(prefix_width);
    qInstallMessageHandler(messageHandler);
    m_appType = appType;
    qDebug() << "App: " DAP_BRAND " " DAP_VERSION " " + appType;
    qDebug() << systemInfo();
    setPathToLog(defaultLogPath(DAP_BRAND));
    QDir dir(m_pathToLog);
    if (!dir.exists()) {
        qDebug() << "dir not exists";
        if (dir.mkpath(m_pathToLog) == false)
          qDebug() << "unable to create dir";
#ifndef Q_OS_ANDROID
        system(("chmod -R 667 " + m_pathToLog).toUtf8().data());
#endif

    }
#ifndef Q_OS_ANDROID
    system(("chmod 667 $(find " + m_pathToLog + " -type d)").toUtf8().data());
#endif
    updateCurrentLogName();
    setLogFile(m_currentLogName);
    clearOldLogs();
    connect(DapLogger::instance(), &DapLogger::sigMessageHandler,
            this, &DapLogger::updateCurrentLogName);

}

DapLogger* DapLogger::instance()
{
    return m_instance;
}

inline dap_log_level DapLogger::castQtMsgToDap(QtMsgType type)
{
    switch (type) {
    case QtDebugMsg:
        return dap_log_level::L_DEBUG;
    case QtInfoMsg:
        return dap_log_level::L_INFO;
    case QtWarningMsg:
        return dap_log_level::L_WARNING;
    case QtCriticalMsg:
        return dap_log_level::L_ERROR;
    case QtFatalMsg: // Qt FatalMsg as L'CRITICAL interrupt the program by abort() function
        return dap_log_level::L_CRITICAL;
    }
    std::runtime_error("Can't cast QtMsg");
    return dap_log_level::L_CRITICAL; // fix compile warning
}

void DapLogger::setLogLevel(dap_log_level ll)
{
    dap_log_level_set(ll);
}

void DapLogger::setLogFile(const QString& fileName)
{
    if(isLoggerStarted)
        dap_common_deinit();

    qDebug() << "setLogFile: " + fileName;
    QString filePath = getPathToLog() + "/" + fileName;
    dap_common_init(DAP_BRAND, qPrintable(filePath), qPrintable(getPathToLog()));
    DapDataLocal::instance()->setLogPath(getPathToLog());
    DapDataLocal::instance()->setLogFilePath(filePath);
    isLoggerStarted = true;
}

void DapLogger::updateLogFiles()
{
    QString currentDay = QDateTime::currentDateTime().toString("dd");
    if (currentDay == m_day)
        return;
    m_day = currentDay;
    this->updateCurrentLogName();
    this->setLogFile(m_currentLogName);
    this->clearOldLogs();
}

QString DapLogger::defaultLogPath(const QString a_brand)
{
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    return QString("/var/log/%1").arg(a_brand).toLower();
#elif defined(Q_OS_MACOS)
    return QString("/var/log/%1").arg(a_brand).toLower();
#elif defined (Q_OS_WIN)
    return QString("%1/%2/log").arg(regWGetUsrPath()).arg(DAP_BRAND);
#elif defined Q_OS_ANDROID
    Q_UNUSED(a_brand);
    static QAndroidJniObject l_pathObj = QtAndroid::androidContext().callObjectMethod(
                    "getFilesDir"
                    , "()Ljava/io/File;");
    return QString("%1/log").arg(l_pathObj.toString());
#endif
    return {};
}

QString DapLogger::currentLogFileName(const QString a_brand, const QString a_appType)
{
    return QString("%1%2_%3.log").arg(a_brand).arg(a_appType).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy"));
}

QString DapLogger::currentLogFilePath(const QString a_brand, const QString a_appType)
{
    return QString("%1/%2").arg(DapLogger::defaultLogPath(a_brand)).arg(DapLogger::currentLogFileName(a_brand, a_appType));
}

void DapLogger::updateCurrentLogName()
{
    m_currentLogName = DapLogger::currentLogFileName(DAP_BRAND, m_appType);
}


void DapLogger::clearOldLogs()
{

    QDir dir(m_pathToLog);

    if (!dir.exists()) {
        qWarning("The directory does not exist");
        return;
    }

    QFileInfoList list = dir.entryInfoList();
    QDateTime deleteDate = QDateTime::currentDateTime().addDays(-2);
    for (auto file : list){
        if (file.lastModified() < deleteDate){
            dir.remove(file.fileName());
        }
    }
}

void DapLogger::messageHandler(QtMsgType type,
                               const QMessageLogContext &ctx,
                               const QString & msg)
{
    emit DapLogger::instance()->sigMessageHandler();
    DapLogger::instance()->updateLogFiles();
    writeMessage(type, ctx, msg);
}

void DapLogger::writeMessage(QtMsgType type,
                             const QMessageLogContext &ctx,
                             const QString & msg)
{
    if(ctx.file) {
        char prefixBuffer[128];
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
        const char *fileName = strrchr(ctx.file, '/');
#elif defined(Q_OS_WIN)
        const char *fileName = strrchr(ctx.file, '\\');
#else
        #error "Not supported platform"
#endif
        fileName = (fileName == Q_NULLPTR ? ctx.file : fileName + 1);
        strcpy(prefixBuffer, fileName);
        auto dest = strrchr(prefixBuffer, '.');
        if (dest == nullptr)
          dest    = prefixBuffer + strlen(prefixBuffer);
        sprintf(dest, ":%d", ctx.line);

        _log_it(prefixBuffer, castQtMsgToDap(type), "%s", qUtf8Printable(msg));
    } else {
        _log_it("\0", castQtMsgToDap(type), "%s", qUtf8Printable(msg));
    }
    printf("%s\n",qUtf8Printable(msg));

    std::cerr.flush();
    std::cout.flush();
}

QString DapLogger::systemInfo()
{
    return "Kernel: " + QSysInfo::kernelType()
           + " " + QSysInfo::kernelVersion()
           + " Product: " +   QSysInfo::productType()
           + " Version: " +   QSysInfo::productVersion()
           + " (" + QSysInfo::prettyProductName() + ") "
           + " Cpu architecture build: " +   QSysInfo::buildCpuArchitecture()
           + " Current: " +  QSysInfo::currentCpuArchitecture()
           + " Machine host name: " +  QSysInfo::machineHostName();
}
