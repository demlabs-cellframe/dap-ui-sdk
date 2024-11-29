#include "UserConfigManager.h"
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#include <aclapi.h>
#else
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

UserConfigManager::UserConfigManager(const QString& processName)
    : processName(processName) {

    QString standardPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    qInfo() << "Default settings path:" << standardPath;
}

bool UserConfigManager::configure() {
    QString guiUser = getGuiUser();
    if (guiUser.isEmpty()) {
        qDebug() << "Failed to retrieve GUI user.";
        return false;
    }
    qDebug() << "GUI user:" << guiUser;

    QString userConfigPath = getUserConfigPath(guiUser);
    if (userConfigPath.isEmpty()) {
        qDebug() << "Failed to determine user config path.";
        return false;
    }
    qDebug() << "User config path:" << userConfigPath;

    if (!checkFolderExists(userConfigPath)) {
        qDebug() << "Config path does not exist:" << userConfigPath;
        return false;
    }

#ifdef Q_OS_WIN
    if (!setPermissionsWindows(userConfigPath, true)) {
        qDebug() << "Failed to change permissions for:" << userConfigPath;
        return false;
    }
#else
    if (!changeOwnershipRecursively(userConfigPath, guiUser)) {
        qDebug() << "Failed to change ownership for:" << userConfigPath;
        return false;
    }
    if (!setReadWritePermissionsRecursively(userConfigPath)) {
        qDebug() << "Failed to change permissions for:" << userConfigPath;
        return false;
    }
#endif

    qDebug() << "Permissions successfully updated for:" << userConfigPath;
    return true;
}

QString UserConfigManager::getGuiUser() const {
    QProcess process;
    process.start(processName, QStringList());
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    QStringList lines = output.split('\n');
    for (const QString& line : lines) {
        if (line.contains(":0")) { // Check if the session is GUI-based
            QStringList parts = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
            if (!parts.isEmpty()) {
                return parts[0];
            }
        }
    }
    return QString();
}

QString UserConfigManager::getUserConfigPath(const QString& user) const {
#ifdef Q_OS_WIN
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "\\" + DAP_BRAND;
#else
    struct passwd* pw = getpwnam(user.toUtf8().constData());
    if (!pw) {
        qDebug() << "Failed to retrieve home directory for user:" << user;
        return QString();
    }
    return QString::fromUtf8(pw->pw_dir) + "/.config/" + DAP_BRAND;
#endif
}

bool UserConfigManager::checkFolderExists(const QString& folderPath) const {
    QDir dir(folderPath);
    return dir.exists();
}

bool UserConfigManager::changeOwnershipRecursively(const QString& folderPath, const QString& user) const {
    struct passwd* pw = getpwnam(user.toUtf8().constData());
    if (!pw) {
        qDebug() << "Failed to retrieve user information for:" << user;
        return false;
    }

    if (chown(folderPath.toUtf8().constData(), pw->pw_uid, pw->pw_gid) != 0) {
        perror(("Failed to change ownership for folder: " + folderPath).toUtf8().constData());
        return false;
    }

    QDirIterator it(folderPath, QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        if (chown(path.toUtf8().constData(), pw->pw_uid, pw->pw_gid) != 0) {
            perror(("Failed to change ownership for: " + path).toUtf8().constData());
            return false;
        }
    }

    return true;
}

bool UserConfigManager::setReadWritePermissionsRecursively(const QString& folderPath) const {
    mode_t mode_dir = S_IRUSR | S_IWUSR | S_IXUSR;
    mode_t mode_file = S_IRUSR | S_IWUSR;

    if (chmod(folderPath.toUtf8().constData(), mode_dir) != 0) {
        perror(("Failed to change permissions for folder: " + folderPath).toUtf8().constData());
        return false;
    }

    QDirIterator it(folderPath, QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();

        QFileInfo fileInfo(path);
        mode_t mode = fileInfo.isDir() ? mode_dir : mode_file;

        if (chmod(path.toUtf8().constData(), mode) != 0) {
            perror(("Failed to change permissions for: " + path).toUtf8().constData());
            return false;
        }
    }

    return true;
}

#ifdef Q_OS_WIN

bool UserConfigManager::setPermissionsWindows(const QString& folderPath, bool isDirectory) const {
    DWORD permissions = GENERIC_READ | GENERIC_WRITE;
    DWORD attributes = isDirectory ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL;

    if (SetFileAttributesW(reinterpret_cast<LPCWSTR>(folderPath.utf16()), attributes) == 0) {
        qDebug() << "Failed to set attributes for:" << folderPath;
        return false;
    }

    qDebug() << "Permissions updated for:" << folderPath;
    return true;
}

#endif
