#ifndef DAPCMDRESETSEIALKEY_H
#define DAPCMDRESETSEIALKEY_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdClientAbstract.h"

class DapCmdResetSeialKey : public DapCmdClientAbstract
{
    Q_OBJECT

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

public:

    explicit DapCmdResetSeialKey(QObject *parent = nullptr);
    virtual ~DapCmdResetSeialKey() override { }

    void sendResetSerialKeyRequest(const QString &a_serial = "");
signals:
    void sigResetSerialKeyError(const QString& error);
    void sigResetSerialKeyErrorSetOnlyMessage(const QString& error);
    void sigSerialKeyReseted();
};

#endif // DAPCMDRESETSEIALKEY_H
