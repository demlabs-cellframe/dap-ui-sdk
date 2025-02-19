#include "DapJsonCmdController.h"
#include <QDebug>

DapJsonCmdController::DapJsonCmdController(QObject *parent)
    : QObject(parent), m_handlers(0)
{

}

DapCmdAbstract* DapJsonCmdController::findHandler(DapJsonCmdType cmd)
{
    for(DapCmdAbstract* handler : m_handlers) {
        if(handler->cmd() == cmd)
            return handler;
    }
    return Q_NULLPTR;
}

void DapJsonCmdController::handleCmd(const QByteArray &a_cmd)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(a_cmd, &err);

    if (err.error == QJsonParseError::IllegalUTF8String ){
        QString s = QString::fromUtf8(a_cmd);
        if (s.toUtf8() != a_cmd) {
          s = QString::fromLatin1(a_cmd);
        }
        doc = QJsonDocument::fromJson(s.toUtf8(), &err);
    }

    if (doc.isEmpty()) {
        qCritical() << "Bad command" << a_cmd;
        return;
    }

    DapJsonCmdType cmd = DapCmdAbstract::getCommand(doc);
    auto handler = findHandler(cmd);
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
    Q_ASSERT_X(handler, "[addNewHandler] handler not valid", 0);
    if(findHandler(handler->cmd()) != Q_NULLPTR) {
        qCritical() << "Handler for command" << DapCmdAbstract::commandToString(handler->cmd())
                    << "already exists";
        return;
    }
    connect(handler, &DapCmdAbstract::send, this, &DapJsonCmdController::sendDapCmd);

    m_handlers.append(handler);
}
