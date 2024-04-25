#ifndef DapCmdUpdateOperationSrv_H
#define DapCmdUpdateOperationSrv_H

#include <QObject>
#include <QJsonValue>
#include <QDateTime>

#include "DapCmdServiceAbstract.h"

class DapCmdUpdateOperationSrv: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdUpdateOperationSrv(QObject *parent = nullptr);
    virtual ~DapCmdUpdateOperationSrv() override {}

    void handle(const QJsonObject *params) override;

public slots:
    void setDownloadProgress(quint64 load, quint64 total);
signals:
    void startDownload(QString url, QString path);
    void startUpdate();
};

#endif // DapCmdUpdateOperationSrv_H
