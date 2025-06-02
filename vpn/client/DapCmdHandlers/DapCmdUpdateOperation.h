#ifndef DAPCMDUPDATEOPERATION_H
#define DAPCMDUPDATEOPERATION_H

#include <QObject>
#include <QJsonValue>
#include <QDateTime>

#include "DapCmdServiceAbstract.h"

class DapCmdUpdateOperation: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdUpdateOperation(QObject *parent = nullptr);
    virtual ~DapCmdUpdateOperation() override {}

    void handle(const QJsonObject *params) override;

public slots:
    void setDownloadProgress(quint64 load, quint64 total);
signals:
    void startDownload(QString url, QString path);
    void startUpdate();
    void removeDownload(const QString& downloadFileName);
};

#endif // DAPCMDUPDATEOPERATION_H
