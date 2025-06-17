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

#ifdef Q_OS_ANDROID
#include <DapAndroidHelpers.h>
#include <android/log.h>
#endif

#ifdef Q_OS_IOS
#include "DapIOSLogger.h"
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
#ifndef Q_OS_IOS
        system(("chmod -R 777 " + m_pathToLog).toUtf8().data());
#endif
#endif
    }
#ifndef Q_OS_ANDROID
#ifndef Q_OS_IOS
    system(("chmod 777 $(find " + m_pathToLog + " -type d)").toUtf8().data());
#endif
#endif
    updateCurrentLogName();
    setLogFile(m_currentLogName);
    clearOldLogs();
//    connect(DapLogger::instance(), &DapLogger::sigMessageHandler,
//            this, &DapLogger::updateCurrentLogName);

    auto then = QDateTime::currentDateTime();
    auto setTime = QTime::fromString("00:00", "hh:mm");
    if(then.time() > setTime){
        then = then.addDays(1);
    }
    then.setTime(setTime);

    auto diff = QDateTime::currentDateTime().msecsTo(then);

    QTimer::singleShot(diff, [this]{
        auto t = new QTimer(QCoreApplication::instance());
        connect(t, &QTimer::timeout, [this]{
            DapLogger::instance()->updateCurrentLogName();
            DapLogger::instance()->updateLogFiles();
        });
        t->start(24 * 3600 * 1000);
    });

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
    dap_common_init(DAP_BRAND, qPrintable(filePath));
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
    static DapQtJniObject l_pathObj = dapQtAndroidServiceContext().callObjectMethod(
                    "getFilesDir"
                    , "()Ljava/io/File;");
    return QString("%1/log").arg(l_pathObj.toString());
#elif defined (Q_OS_IOS)
    return QString(dapIOSLogPath());
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
                               const QMessageLogContext &context,
                               const QString & msg)
{
   #ifdef Q_OS_ANDROID
  QString report=msg;
  if (context.file && !QString(context.file).isEmpty()) {
    report+=" in file ";
    report+=QString(context.file);
    report+=" line ";
    report+=QString::number(context.line);
  }
  if (context.function && !QString(context.function).isEmpty()) {
    report+=+" function ";
    report+=QString(context.function);
  }
  const char*const local=report.toLocal8Bit().constData();
  switch (type) {
  case QtDebugMsg:
    __android_log_write(ANDROID_LOG_DEBUG,DAP_BRAND,local);
    break;
  case QtInfoMsg:
    __android_log_write(ANDROID_LOG_INFO,DAP_BRAND,local);
    break;
  case QtWarningMsg:
    __android_log_write(ANDROID_LOG_WARN,DAP_BRAND,local);
    break;
  case QtCriticalMsg:
    __android_log_write(ANDROID_LOG_ERROR,DAP_BRAND,local);
    break;
  case QtFatalMsg:
  default:
    __android_log_write(ANDROID_LOG_FATAL,DAP_BRAND,local);
    abort();    
  }
  #endif
    writeMessage(type, context, msg);
}

void DapLogger::writeMessage(QtMsgType type,
                             const QMessageLogContext &ctx,
                             const QString & msg)
{
    if(ctx.file) {
        char prefixBuffer[128];
#if defined(Q_OS_LINUX) || defined(Q_OS_DARWIN)
        const char *fileName = strrchr(ctx.file, '/');
#elif defined(Q_OS_WIN)
        const char *fileName = strrchr(ctx.file, '\\');
#else
        #error "Not supported platform"
#endif
        fileName = (fileName == Q_NULLPTR ? ctx.file : fileName + 1);
        strncpy (prefixBuffer, fileName, 128);
        auto dest = strrchr(prefixBuffer, '.');
        if (dest == nullptr)
          dest    = prefixBuffer + strlen(prefixBuffer);
        sprintf(dest, ":%d", ctx.line);

        _log_it(nullptr, 0, prefixBuffer, castQtMsgToDap(type), "%s", qUtf8Printable(msg));
    } else {
        _log_it(nullptr, 0, "\0", castQtMsgToDap(type), "%s", qUtf8Printable(msg));
    }
    //printf("%s\n",qUtf8Printable(msg));

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
