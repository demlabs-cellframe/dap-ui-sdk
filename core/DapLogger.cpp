#include <stdio.h>
#include "dap_common.h"
#include "dap_file_utils.h"
#include "DapLogger.h"
#include "DapDataLocal.h"
#include <iostream>
#ifdef Q_OS_WIN
#include "registry.h"
#endif

DapLogger::DapLogger(QObject *parent, QString appType, size_t prefix_width)
    : QObject(parent)
{
    dap_set_log_tag_width(prefix_width);
    qInstallMessageHandler(messageHandler);

    m_appType = appType;

    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, &DapLogger::resetLogFileIfNotExist);
    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &DapLogger::resetLogDirIfNotExist);
}

inline dap_log_level DapLogger::castQtMsgToDap(QtMsgType type) {
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

void DapLogger::setLogLevel(dap_log_level ll) {
    dap_log_level_set(ll);
}

bool DapLogger::setLogFile(const QString& filePath) {
    qDebug() << "setLogFile: " << filePath;
    int i = dap_common_init(DAP_BRAND, qPrintable(filePath)) ;
    DapDataLocal::instance()->setLogFilePath(filePath);
    m_watcher->addPath(filePath);
    return i == 0;
}

int DapLogger::createLogFolder(QString path){
    int ret = dap_mkdir_with_parents(qPrintable(path));
    setPermissionFolder(path);
    m_watcher->addPath(path.replace("/log", ""));
    m_watcher->addPath(path.replace(QString("/" DAP_BRAND).toLower(), ""));
    return ret;
}
void DapLogger::setPermissionFolder(const QString& path)
{
#if !defined (Q_OS_WIN)
    system(qPrintable("chown -R $(logname):$(logname) " + path));
#endif
}

void DapLogger::createChangerLogFiles(){

    auto then = QDateTime::currentDateTime();
    auto setTime = QTime::fromString("00:00", "hh:mm");
    if(then.time() > setTime){
        then = then.addDays(1);
    }
    then.setTime(setTime);

    auto diff = QDateTime::currentDateTime().msecsTo(then);
    t.start(diff);
    connect(&t, &QTimer::timeout, [&]{
        t.setInterval(24 * 3600 * 1000);
        m_watcher->removePath(QString("%1/%2").arg(m_pathToLog).arg(m_currentLogName));
        this->updateCurrentLogName();
        this->setLogFile(QString("%1/%2").arg(m_pathToLog).arg(m_currentLogName));
        this->clearOldLogs();
    });
}

void DapLogger::resetLogFileIfNotExist(const QString& path)
{
    QFileInfo check_file(path);
    if (!(check_file.exists() && check_file.isFile())) {
        m_watcher->removePath(path);
        setLogFile(path);
    }
}

void DapLogger::resetLogDirIfNotExist(const QString& path)
{
    QDir check_dir(getPathToLog());
    if (!check_dir.exists()) {
        m_watcher->removePath(path);
        createLogFolder(getPathToLog());
        resetLogFileIfNotExist(QString("%1/%2").arg(getPathToLog()).arg(getCurrentLogName()));
    }
}

QString DapLogger::defaultLogPath(const QString a_brand)
{
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    return QString("/opt/%1/log").arg(a_brand).toLower();
#elif defined(Q_OS_MACOS)
    return QString("/tmp/%1/log").arg(a_brand);
#elif defined (Q_OS_WIN)
    return QString("%1/%2/log").arg(regWGetUsrPath()).arg(DAP_BRAND);
#elif defined Q_OS_ANDROID
    return QString("/sdcard/%1/log").arg(a_brand);
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


void DapLogger::clearOldLogs(){

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
        sprintf(strrchr(prefixBuffer, '.'), ":%d", ctx.line);

        _log_it(prefixBuffer, castQtMsgToDap(type), msg.toLatin1().data());
    } else {
        _log_it("\0", castQtMsgToDap(type), msg.toLatin1().data());
    }
}
