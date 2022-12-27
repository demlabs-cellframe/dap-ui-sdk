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
    void startDownload(QString url);
    void startUpadte();

signals:
    void downloadProgress(qint64 load, qint64 total);
};

#endif // DAPCMDUPDATEOPERATION_H
