#ifndef DAPCMDCONTROLLER_H
#define DAPCMDCONTROLLER_H

#include "DapCmdAbstract.h"
#include <QObject>
#include <QVector>

class DapJsonCmdController : public QObject
{
    Q_OBJECT

public:
    explicit DapJsonCmdController(QObject *parent = nullptr);

    void handleCmd(const QByteArray &a_cmd);
    void addNewHandler(DapCmdAbstract* handler);
signals:
    void sendDapCmd(const QByteArray& ba);

private:
    // returns Q_NULLPTR if handler not found
    DapCmdAbstract* findHandler(DapJsonCmdType cmd);

private:
    QVector<DapCmdAbstract*> m_handlers;
};

#endif // DAPCMDCONTROLLER_H
