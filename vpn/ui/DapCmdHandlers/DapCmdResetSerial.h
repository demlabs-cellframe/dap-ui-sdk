#ifndef DAPCMDRESETSERIAL_H
#define DAPCMDRESETSERIAL_H

#include <QObject>
#include "DapCmdClientAbstract.h"

class DapCmdResetSerial : public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdResetSerial(QObject *parent = nullptr);

public slots:
    void sendCmdResetSerialRequest(const QString &serial);
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
signals:
    void resetSerialAnswer(const QString& answer);
    void resetSerialAnswerIsError(const QString& errorAnswer);
};

#endif // DAPCMDRESETSERIAL_H
