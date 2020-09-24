#ifndef DAPCMDRESETSERIAL_H
#define DAPCMDRESETSERIAL_H

#include <QDebug>
#include "DapCmdServiceAbstract.h"

class DapCmdResetSerial: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdResetSerial(QObject *parent = nullptr);
    void handle(const QJsonObject *params) override;
signals:
    void sigResetSerialAnswer(const QJsonDocument& answer);
    void resetSerialRequest(const QString &serial);
};

#endif // DAPCMDRESETSERIAL_H
