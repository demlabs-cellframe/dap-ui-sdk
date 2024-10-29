#ifndef DAPCMDANDROIDTUNNEL_H
#define DAPCMDANDROIDTUNNEL_H

#include "DapCmdClientAbstract.h"

class DapCmdAndroidTunnel : public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdAndroidTunnel(QObject *parent = nullptr);
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

signals:
    void asked(const QString&, const QString&, const int);
};

#endif // DAPCMDANDROIDTUNNEL_H
