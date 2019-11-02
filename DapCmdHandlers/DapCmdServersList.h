#ifndef DAPCMDSERVERSLIST_H
#define DAPCMDSERVERSLIST_H

#include <QObject>
#include "DapCmdServiceAbstract.h"

class DapCmdServersList: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdServersList(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;
    ~DapCmdServersList() override {}
    const QString& serversList() { return  m_serversList; }
signals:
public slots:
    void setServersList(const QString& a_serversList) { m_serversList = a_serversList ; }
private:
    QString m_serversList;
};

#endif // DAPCMDSERVERSLIST_H
