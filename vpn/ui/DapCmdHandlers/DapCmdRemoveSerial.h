#ifndef DAPCMDREMOVESERIAL_H
#define DAPCMDREMOVESERIAL_H

#include "DapCmdClientAbstract.h"

class DapCmdRemoveSerial: public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdRemoveSerial(QObject *parent = nullptr);
    virtual ~DapCmdRemoveSerial() override { }

    void sendRemoveSerialRequest(const QString &a_serial);

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
signals:
    void serialRemoved();
    void serialNotRemoved();
};

#endif // DAPCMDREMOVESERIAL_H
