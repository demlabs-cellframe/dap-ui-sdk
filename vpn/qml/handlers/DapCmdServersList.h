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
signals:
    void sigServersListUpdate(const DapServerInfoList& servers, QDateTime a_lastUpdateTime);
    void sigEmptyList(const QString &msg);
    void sigErrorNetwork(const QString &msg);
public slots:
    void sendCmdGetServersList() { sendCmd(); }
    void setNoCdbMode(bool enabled) { m_noCdbMode = enabled; }

private:
    bool m_noCdbMode = false;
};

#endif // DAPCMDSERVERSLIST_H
