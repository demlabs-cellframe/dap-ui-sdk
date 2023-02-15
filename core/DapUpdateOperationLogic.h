#ifndef DAPUPDATEOPERATIONLOGIC_H
#define DAPUPDATEOPERATIONLOGIC_H

#include <QObject>

// Logical module for blocking update starting if there was an error when downloading
class DapUpdateOperationLogic : public QObject
{
    Q_OBJECT
public:
    DapUpdateOperationLogic();
    ~DapUpdateOperationLogic(){};
    static DapUpdateOperationLogic *instance();
    void startDownload();
    void setDownloadError();
    bool isDownloadError();
#ifndef Q_OS_ANDROID
    void startUpdate();
#endif
protected:
    bool m_downloadError;
    QString updateApp();
    QString currentApplication();
    QString downloadFileName();
    QString applicationDirPath();

};

#endif // DAPUPDATEOPERATIONLOGIC_H
