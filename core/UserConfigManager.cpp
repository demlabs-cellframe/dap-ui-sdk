#include "UserConfigManager.h"
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>
#include <QRegularExpression>

#ifdef Q_OS_WIN
#include <windows.h>
#include <aclapi.h>
#else
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <cstring>
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

    if (!checkFileExists(userConfigPath)) {
        qDebug() << "Config path does not exist:" << userConfigPath;
        return false;
    }

#ifndef Q_OS_WIN
#ifndef Q_OS_ANDROID
    if (!changeOwnership(userConfigPath, guiUser)) {
        qDebug() << "Failed to change ownership for:" << userConfigPath;
    }
    if (!setReadWritePermissions(userConfigPath)) {
        qDebug() << "Failed to change permissions for:" << userConfigPath;
        return false;
    }
#endif
#endif

    qDebug() << "Permissions successfully updated for:" << userConfigPath;
    return true;
}

QString UserConfigManager::getGuiUser() const {
    QProcess process;
    qDebug() << "Starting process to determine GUI user...";

    process.start(processName, QStringList());
    if (!process.waitForFinished()) {
        qWarning() << "Process failed to finish in a timely manner.";
        return QString();
    }

    QString output = process.readAllStandardOutput();
    qDebug() << "Process output:" << output;

    QStringList lines = output.split('\n');

    // Regular expressions to match session identifiers
    QRegularExpression guiSessionRegex(":\\d+");       // Matches GUI sessions like :0, :1, etc.
    QRegularExpression ttyRegex("tty\\d+");            // Matches tty sessions like tty1, tty2, etc.

    // Priority check for GUI session identifiers
    for (const QString& line : lines) {
        qDebug() << "Processing line:" << line;

        if (line.contains(guiSessionRegex) || line.contains("wayland") || line.contains("seat0")) {
            qDebug() << "Found high-priority GUI session line:" << line;
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (!parts.isEmpty()) {
                qDebug() << "Returning GUI user (high-priority):" << parts[0];
                return parts[0];
            }
        }
    }

    // Secondary check for tty and pts sessions
    for (const QString& line : lines) {
        qDebug() << "Processing line (secondary):" << line;
        if (line.contains(ttyRegex) || line.contains("pts")) {
            qDebug() << "Found secondary session line:" << line;
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (!parts.isEmpty()) {
                qDebug() << "Returning user (secondary):" << parts[0];
                return parts[0];
            }
        }
    }

    qWarning() << "No GUI user found.";
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
    return QString::fromUtf8(pw->pw_dir) + "/.config/" + DAP_BRAND + "/" + DAP_BRAND + ".conf";
#endif
}

bool UserConfigManager::checkFileExists(const QString& filePath) const {
    QFile file(filePath);
    return file.exists();
}

#ifndef Q_OS_WIN
#ifndef Q_OS_ANDROID
bool UserConfigManager::changeOwnership(const QString& targetPath, const QString& user) const {
    struct passwd* pw = getpwnam(user.toUtf8().constData());
    if (!pw) {
        qDebug() << "Error: Failed to retrieve user information for:" << user;
        return false;
    }

    if (chown(targetPath.toUtf8().constData(), pw->pw_uid, pw->pw_gid) != 0) {
        qDebug() << "Error: Failed to change ownership for:" << targetPath
                 << "Error:" << strerror(errno);
        return false;
    }

    qDebug() << "Ownership successfully changed for:" << targetPath;
    return true;
}

bool UserConfigManager::setReadWritePermissions(const QString& targetPath) const {
    mode_t mode_dir = S_IRUSR | S_IWUSR | S_IXUSR;
    mode_t mode_file = S_IRUSR | S_IWUSR;

    QFileInfo fileInfo(targetPath);
    mode_t mode = fileInfo.isDir() ? mode_dir : mode_file;

    if (chmod(targetPath.toUtf8().constData(), mode) != 0) {
        qDebug() << "Error: Failed to change permissions for:" << targetPath
                 << "Error:" << strerror(errno);
        return false;
    }

    qDebug() << "Permissions successfully changed for:" << targetPath;
    return true;
}
#endif
#endif
