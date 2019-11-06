#ifndef DAPCMDSERVERSLIST_H
#define DAPCMDSERVERSLIST_H

#include "DapCmdClientAbstract.h"
#include "DapServerInfo.h"

class DapCmdServersList: public DapCmdClientAbstract
{
    Q_OBJECT
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
public:
    explicit DapCmdServersList(QObject *parent = nullptr);
    void sendCmdGetServersList() { sendCmd(); }
signals:
    void sigServersList(const DapServerInfoList& servers);
};

#endif // DAPCMDSERVERSLIST_H
