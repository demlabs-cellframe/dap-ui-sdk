#ifndef USERCONFIGMANAGER_H
#define USERCONFIGMANAGER_H

#include <QString>

class UserConfigManager {
public:
    explicit UserConfigManager(const QString& processName = "who");

    bool configure();

private:
    QString processName;

    QString getGuiUser() const;

    QString getUserConfigPath(const QString& user) const;

    bool checkFolderExists(const QString& folderPath) const;

#ifdef Q_OS_WIN
    bool setPermissionsWindows(const QString& folderPath, bool isDirectory) const;
#else
    bool changeOwnershipRecursively(const QString& folderPath, const QString& user) const;

    bool setReadWritePermissionsRecursively(const QString& folderPath) const;
#endif

};

#endif // USERCONFIGMANAGER_H
