#ifndef DAPCMDREMOVESERIAL_H
#define DAPCMDREMOVESERIAL_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdRemoveSerial : public DapCmdServiceAbstract
{
    Q_OBJECT

public:
    DapCmdRemoveSerial(QObject *parent = nullptr);
    ~DapCmdRemoveSerial() override;

    void handle(const QJsonObject *params) override;
signals:
    void sendRemoveSerialAnswer(const bool& removed);
    void removeSerial(const QString& answer);

};

#endif // DAPCMDREMOVESERIAL_H
