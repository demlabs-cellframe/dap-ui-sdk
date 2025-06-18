#ifndef DAPCMDUPDATEOPERATION_H
#define DAPCMDUPDATEOPERATION_H

#include <QObject>

#include "DapCmdClientAbstract.h"

class DapCmdUpdateOperation : public DapCmdClientAbstract
{
    Q_OBJECT
    /****************************************//**
     * @name CONSTRUCT/DESTRUCT
     *******************************************/
    /// @{
public:
    explicit DapCmdUpdateOperation(QObject *parent = nullptr):
        DapCmdClientAbstract(DapJsonCmdType::UPDATE_OPERATION, parent) {}
    virtual ~DapCmdUpdateOperation() override {}
    /// @}

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

public slots:
    void startDownload();
    void setDownloadUrl(QString url);
    void setAvailableVersion(QString avVersion);
    void setDownloadPack(QString pack);
    void startDownloadUrl(QString url, QString path);
    void startUpadte();
    void sendDownloadRemove(const QString& downloadPack);

signals:
    void downloadProgress(qint64 load, qint64 total);
    void downloadError(QString message);

private:
    QString m_dowmloadUrl;
    QString m_dowmloadPack;
    QString m_availableVersion;
};

#endif // DAPCMDUPDATEOPERATION_H
