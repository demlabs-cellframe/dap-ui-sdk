#include "QtCore"
#include "DapUpdateOperationLogic.h"

#ifdef Q_OS_MAC
    #include "CoreFoundation/CoreFoundation.h"
#endif


static DapUpdateOperationLogic *s_instance   = nullptr;

DapUpdateOperationLogic::DapUpdateOperationLogic():
    QObject(nullptr),
    m_downloadError(false)
{
    s_instance = this;
}

DapUpdateOperationLogic *DapUpdateOperationLogic::instance()
{
    if (s_instance == nullptr)
      s_instance  = new DapUpdateOperationLogic;
    return s_instance;
}

bool DapUpdateOperationLogic::isDownloadError()
{
    return m_downloadError;
}

void DapUpdateOperationLogic::setDownloadError()
{
    m_downloadError = true;
}

void DapUpdateOperationLogic::startDownload()
{
    m_downloadError = false;
}

#ifndef Q_OS_ANDROID
// start update application for desktop os
void DapUpdateOperationLogic::startUpdate()
{
    qInfo() << "Start update process";
    bool detached = false;
    QString updateAppPath = updateApp();
#ifdef Q_OS_LINUX
    detached = QProcess::startDetached(updateAppPath, QStringList() << "-p" << downloadFileName() << "-a" << currentApplication());
#endif
#ifdef Q_OS_WIN
    updateAppPath = QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "Update.exe");
    fileCopy(updateApp(), updateAppPath);
    QString psArgs = (QStringList() <<  "\"-p\"" << QString("\"%1\"").arg(downloadFileName().replace(" ", "` ")) << "\"-a\"" << QString("\"%1\"").arg(currentApplication().replace(" ", "` "))).join(",");
    detached = QProcess::startDetached("Powershell", QStringList() << "Start" << updateAppPath.replace(" ", "` ") << "-ArgumentList" << psArgs << "-Verb" << "Runas");
    qInfo() << QString("%1 %2").arg("Powershell", (QStringList() << "Start" << updateAppPath << "-ArgumentList" << psArgs << "-Verb" << "Runas").join(" "));
#endif
#ifdef Q_OS_MACOS
    updateAppPath = QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "Update");
    fileCopy(updateApp(), updateAppPath);
    detached = QProcess::startDetached(updateAppPath, QStringList() << "-p" << downloadFileName() << "-a" << currentApplication());
#endif
    if(!detached)
        qWarning() << "Failed to start update agent application" << updateAppPath;
    else
        qInfo() << "Start update agent application" << updateAppPath << downloadFileName() << currentApplication();
}
#endif

// update app path
QString DapUpdateOperationLogic::updateApp()
{
    QString updateAgent = applicationDirPath();
#if defined(QT_DEBUG)
    updateAgent = qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "DapChainVpnUpdateApp";
#endif
#ifdef Q_OS_LINUX
    return updateAgent + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "Update");
#endif
#ifdef Q_OS_WIN
    return updateAgent + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "Update.exe");
#endif
#ifdef Q_OS_MACOS
    return updateAgent + QDir::separator() + pathInsideMacOSPack(DAP_BRAND)
            + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "Update");
#endif
}

// download file path
QString DapUpdateOperationLogic::downloadFileName()
{
#ifdef Q_OS_LINUX
    return QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "-update.deb"); // example KelVPN-installer.deb
#endif
#ifdef Q_OS_WIN
    return QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "-update.exe"); // example "KelVPN-installer.exe"
#endif
#ifdef Q_OS_MACOS
    return QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND, "-update.pkg"); // example "KelVPN-installer.pkg"
#endif
}

// this app path
QString DapUpdateOperationLogic::currentApplication()
{
#ifdef Q_OS_LINUX
    return applicationDirPath() + QDir::separator() + QString(DAP_BRAND);
#endif
#ifdef Q_OS_WIN
    return applicationDirPath() + QDir::separator() + QString("%1.exe").arg(DAP_BRAND);
#endif
#ifdef Q_OS_MAC
    return applicationDirPath() + pathInsideMacOSPack(DAP_BRAND)
            + QDir::separator() + QString("%1").arg(DAP_BRAND);
#endif
}

// this app folder
QString DapUpdateOperationLogic::applicationDirPath()
{
#ifdef Q_OS_MAC
    QString appName = "Update.app";
    CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef,kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath,CFStringGetSystemEncoding());
    CFRelease(pluginRef);
    CFRelease(macPath);
    QString filePath = QString(pathPtr);
    QString applicationDirPath = filePath.left(filePath.lastIndexOf("/"));
    return applicationDirPath;
#else
    return qApp->applicationDirPath();
#endif
}

QString DapUpdateOperationLogic::pathInsideMacOSPack(QString packName)
{
    return QString("/%1.app/Contents/MacOS").arg(packName);
}

bool DapUpdateOperationLogic::fileCopy(QString source, QString dest)
{
    if (QFile::exists(dest))
        QFile::remove(dest);
    return QFile::copy(source, dest);
}
