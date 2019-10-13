#include "DapJsonCmdController.h"
#include <QDebug>

DapJsonCmdController::DapJsonCmdController(QObject *parent)
    : QObject(parent), m_handlers(0)
{

}

DapCmdAbstract* DapJsonCmdController::_findHandler(DapJsonCmdType cmd)
{
    for(DapCmdAbstract* handler : m_handlers) {
        if(handler->cmd() == cmd)
            return handler;
    }
    return Q_NULLPTR;
}

void DapJsonCmdController::handleCmd(const QByteArray &a_cmd)
{
    QJsonDocument doc = QJsonDocument::fromJson(a_cmd);
    if (doc.isEmpty()) {
        qCritical() << "Bad command" << a_cmd;
        return;
    }

    DapJsonCmdType cmd = DapCmdAbstract::getCommand(doc);
    auto handler = _findHandler(cmd);
    if (handler == Q_NULLPTR) {
        qWarning() << "Not found handler for command "
                   << DapCmdAbstract::commandToString(cmd);
        return;
    }

    QString objName = doc.object().contains("result") ? "result" :
        doc.object().contains("params") ? "params" : "";

    QJsonObject obj = doc.object()[objName].toObject();
    handler->handle(&obj);
}

void DapJsonCmdController::addNewHandler(DapCmdAbstract* handler)
{
    if(_findHandler(handler->cmd()) != Q_NULLPTR) {
        qCritical() << "Handler for command" << DapCmdAbstract::commandToString(handler->cmd())
                    << "already exists";
        return;
    }
    qDebug() << "Added handler for command" << DapCmdAbstract::commandToString(handler->cmd());
    connect(handler, &DapCmdAbstract::send,
            this, &DapJsonCmdController::sendDapCmd);

    m_handlers.append(handler);
}
