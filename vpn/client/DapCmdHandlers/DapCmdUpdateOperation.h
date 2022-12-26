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
    DapCmdUpdateOperation(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::GET_USER_DATA, parent) { }
public slots:
    void setDownloadProgress(qreal progress);
signals:
    void startDownload(QString url);
    void startUpdate();
};

#endif // DAPCMDUPDATEOPERATION_H
