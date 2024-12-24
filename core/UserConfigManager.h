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

    bool checkFileExists(const QString& folderPath) const;

#ifndef Q_OS_WIN
    bool changeOwnership(const QString& folderPath, const QString& user) const;

    bool setReadWritePermissions(const QString& folderPath) const;
#endif

};

#endif // USERCONFIGMANAGER_H
