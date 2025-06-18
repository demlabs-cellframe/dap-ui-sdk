#ifndef DapCmdResetSerialKey_H
#define DapCmdResetSerialKey_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdClientAbstract.h"

class DapCmdResetSerialKey : public DapCmdClientAbstract
{
    Q_OBJECT

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

public:

    explicit DapCmdResetSerialKey(QObject *parent = nullptr);
    virtual ~DapCmdResetSerialKey() override { }

    void sendResetSerialKeyRequest(const QString &a_serial = "");
signals:
    void sigResetSerialKeyError(const QString& error);
    void sigResetSerialKeyErrorSetOnlyMessage(const QString& error);
    void sigSerialKeyReseted();
};

#endif // DapCmdResetSerialKey_H
