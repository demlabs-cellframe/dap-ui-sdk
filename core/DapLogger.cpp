#include <stdio.h>
#include <QStandardPaths>
#include <iostream>
#include "dap_common.h"
#include "dap_file_utils.h"
#include "DapLogger.h"

#ifdef Q_OS_WIN
#include "registry.h"
#endif

static DapLogger* m_instance = nullptr;
QString DapLogger::m_pathToLog = "";
QString DapLogger::m_pathToFile = "";

DapLogger::DapLogger(QObject *parent, QString appType, size_t prefix_width, TypeLogCleaning typeClean, QString customLogPath)
    : QObject(parent)
    , m_day(QDateTime::currentDateTime().toString("dd"))
    , m_typeLogCleaning(typeClean)
{
    m_instance = this;

    if(TypeLogCleaning::FULL_FILE_SIZE == m_typeLogCleaning)
    {
        // QTimer is created with parent to ensure auto-deletion
        m_timeUpdate = new QTimer(this);
        connect(m_timeUpdate,  &QTimer::timeout, this, &DapLogger::updateLogFilesInfo);
        m_timeUpdate->start(m_timeOut);
    }

    dap_set_log_tag_width(prefix_width);
    qInstallMessageHandler(messageHandler);
    m_appType = appType;
    qDebug() << "App: " DAP_BRAND " " DAP_VERSION " " + appType;
    qDebug() << systemInfo();

    if(!customLogPath.isEmpty())
    {
        setPathToLog(customLogPath);
    }
    else
    {
        setPathToLog(defaultLogPath(DAP_BRAND));
    }

    QDir dir(m_pathToLog);
    if (!dir.exists())
    {
        qDebug() << "dir not exists";
        if (dir.mkpath(m_pathToLog) == false) qDebug() << "unable to create dir";
    #ifndef Q_OS_ANDROID
    #ifdef TYPE_SERVICE
        system(("chmod -R 777 " + m_pathToLog).toUtf8().data());
    #endif
    #endif
    }

#ifndef Q_OS_ANDROID
#ifdef TYPE_SERVICE
    system(("chmod 777 $(find " + m_pathToLog + " -type d)").toUtf8().data());
#endif
#endif

    m_currentDate = getCurrentDate();

    updateCurrentLogName();
    setLogFile(m_currentLogName);
    clearOldLogs();

    auto then = QDateTime::currentDateTime();
    auto setTime = QTime::fromString("00:00", "hh:mm");
    if(then.time() > setTime){
        then = then.addDays(1);
    }
    then.setTime(setTime);

    auto diff = QDateTime::currentDateTime().msecsTo(then);

    QTimer::singleShot(diff, [this]{
        auto t = new QTimer(QCoreApplication::instance());
        connect(t, &QTimer::timeout, this, []{
            DapLogger::instance()->updateCurrentLogName();
            DapLogger::instance()->updateLogFiles();
        });
        t->start(24 * 3600 * 1000);
    });
}

DapLogger::~DapLogger()
{
    if(m_timeUpdate) delete m_timeUpdate;
}

void DapLogger::deleteLogger()
{
    delete m_instance;
    m_instance = nullptr;
}

DapLogger* DapLogger::instance()
{
    return m_instance;
}

void DapLogger::startUpdateTimer()
{
    if(m_timeUpdate)
    {
        m_timeUpdate->start(m_timeOut);
    }
}

void DapLogger::setPathToLog(QString path)
{
    qDebug() << path;
    m_pathToLog = path;
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
    qDebug() << "setLogFile: " + fileName;

    m_pathToFile = getPathToLog() + "/" + fileName;
    dap_common_init(DAP_BRAND, qPrintable(m_pathToFile));

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

    static QJniObject activity = QNativeInterface::QAndroidApplication::context();
    QJniObject filesDir = activity.callObjectMethod("getFilesDir", "()Ljava/io/File;");

    return QString("%1/log").arg(filesDir.toString());
#endif
    return {};
}

QString DapLogger::currentLogFileName(const QString a_brand, const QString a_appType)
{
    if(TypeLogCleaning::FULL_FILE_SIZE == DapLogger::instance()->getTypeLogCleaning())
    {
        return QString("%1%2_%3(%4).log").arg(a_brand).arg(a_appType)
            .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy"))
            .arg(QString::number(DapLogger::instance()->getCurrentIndex()));
    }
    return QString("%1%2_%3.log").arg(a_brand).arg(a_appType).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy"));
}

QString DapLogger::currentLogFilePath(const QString a_brand, const QString a_appType)
{
    return QString("%1/%2").arg(DapLogger::defaultLogPath(a_brand)).arg(DapLogger::currentLogFileName(a_brand, a_appType));
}

void DapLogger::updateCurrentLogName()
{
    if(TypeLogCleaning::FULL_FILE_SIZE == m_typeLogCleaning)
    {
        updateActualIndex();
    }
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
    for (const auto& file : qAsConst(list)){
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
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
        const char *fileName = strrchr(ctx.file, '/');
#elif defined(Q_OS_WIN)
        const char *fileName = strrchr(ctx.file, '\\');
#elif defined(Q_OS_IOS)
        const char *fileName = strrchr(ctx.file, '/');
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

void DapLogger::updateLogFilesInfo()
{
    if (m_pathToLog.isEmpty()) {
        qWarning() << "[DapLogger] Log path is empty, skipping update";
        return;
    }

    QDir dir(m_pathToLog);
    if (!dir.exists() || !dir.isReadable()) {
        qWarning() << "[DapLogger] Log directory is not accessible:" << m_pathToLog;
        return;
    }

    // Check current log file
    bool foundCurrentLog = false;
    for (const QFileInfo &fileInfo : dir.entryInfoList({ m_currentLogName }, QDir::Files)) {
        foundCurrentLog = true;

        if (fileInfo.size() > m_partSize || m_currentDate != getCurrentDate()) {
            qDebug() << "[DapLogger] Rotating log file. Size:" << fileInfo.size()
            << " Name:" << m_currentLogName;

            updateCurrentLogName();
            setLogFile(m_currentLogName);
        }
    }

    if (!foundCurrentLog) {
        qDebug() << "[DapLogger] Log file not found. Creating new log:" << m_currentLogName;
        updateCurrentLogName();
        setLogFile(m_currentLogName);
    }

    // Collect all log files matching mask
    QStringList mask = m_appType == "GUI" ?  QStringList()<<"*GUI*" : QStringList()<<"*Service*";
    QFileInfoList files;
    qint64 totalSize = 0;

    foreach (QFileInfo fileInfo, dir.entryInfoList(mask, QDir::Files))
    {
        totalSize += fileInfo.size();
        files.append(fileInfo);
    }

    // Clean old logs if total size exceeds limit
    if (totalSize > m_maxSize) {
        std::sort(files.begin(), files.end(), [](const QFileInfo &a, const QFileInfo &b) {
            return a.lastModified() < b.lastModified();
        });

        while (totalSize > m_maxSize && !files.isEmpty()) {
            const QFileInfo &file = files.first();
            qint64 fileSize = file.size();
            if (QFile::remove(file.filePath())) {
                totalSize -= fileSize;
                files.removeFirst();
                qDebug() << "[DapLogger] Deleted log file:" << file.filePath();
            } else {
                qWarning() << "[DapLogger] Failed to delete file:" << file.filePath();
                break;
            }
        }
    }
}

void DapLogger::updateActualIndex()
{
    if(m_pathToLog.isEmpty())
    {
        return;
    }
    QDir dir(m_pathToLog);
    QStringList files;
    QString baseFileName = QString("%1%2_%3").arg(DAP_BRAND).arg(m_appType).arg(getCurrentDate());
    foreach (QFileInfo fileInfo, dir.entryInfoList(QStringList() <<
                                                       QString("*%1*").arg(baseFileName), QDir::Files))
    {
        files.append(fileInfo.fileName());
    }

    if(files.isEmpty())
    {
        m_curentIndex = 0;
    }

    int maxIndex = 0;
    for(const QString &item: files)
    {
        QRegularExpressionMatch match = INDEX_EXEPTION.match(item);
        if (match.hasMatch())
        {
          int currentIndex = match.captured(1).toInt();
          if(currentIndex > maxIndex)
          {
              maxIndex = currentIndex;
          }
        }
    }
    baseFileName += QString("(%1)").arg(QString::number(maxIndex));

    qint64 fileSize;
    foreach (QFileInfo fileInfo, dir.entryInfoList(QStringList() <<
                                                       QString("*%1*").arg(baseFileName), QDir::Files))
    {
        fileSize = fileInfo.size();
    }

    m_curentIndex = fileSize >= m_partSize ? maxIndex + 1 : maxIndex;
}

QString DapLogger::getCurrentDate()
{
    return QDateTime::currentDateTime().toString("dd-MM-yyyy");
}
