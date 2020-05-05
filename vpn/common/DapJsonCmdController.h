#ifndef DAPCMDCONTROLLER_H
#define DAPCMDCONTROLLER_H

#include "DapCmdAbstract.h"
#include <QObject>
#include <QVector>

class DapJsonCmdController : public QObject
{
    Q_OBJECT
private:
    // returns Q_NULLPTR if handler not found
    DapCmdAbstract* _findHandler(DapJsonCmdType cmd);

    QVector<DapCmdAbstract*> m_handlers;
public:
    explicit DapJsonCmdController(QObject *parent = nullptr);

    void handleCmd(const QByteArray &a_cmd);
    void addNewHandler(DapCmdAbstract* handler);
signals:
    void sendDapCmd(const QByteArray& ba);
public slots:
};

#endif // DAPCMDCONTROLLER_H
