#ifndef DAPCMDTUNTAPHANDLER_H
#define DAPCMDTUNTAPHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdClientAbstract.h"

class DapCmdTunTap: public DapCmdClientAbstract
{
    Q_OBJECT
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
public:
    explicit DapCmdTunTap(QObject *parent = nullptr):
        DapCmdClientAbstract(DapJsonCmdType::TAP_DRIVER, parent) {}
    ~DapCmdTunTap() override {}

    enum Action { CHECK, INSTALL };
    void sendCmd(Action act);
signals:
    void sigTapOk();
    void sigTapError();
};

#endif // DAPCMDTUNTAPHANDLER_H
