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
    static QString currentApplication();
    static QString downloadFileName();
    static QString applicationDirPath();
    static QString pathInsideMacOSPack(QString pachName);
    static bool fileCopy(QString source, QString dest);


protected:
    bool m_downloadError;
    QString updateApp();

signals:
    void updateComplete();
};

#endif // DAPUPDATEOPERATIONLOGIC_H
