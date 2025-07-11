#include "DapCmdConnectionProblem.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

DapCmdConnectionProblem::DapCmdConnectionProblem(QObject *parent)
    : DapCmdAbstract(Side::CLIENT, DapJsonCmdType::CONNECTION_PROBLEM, parent)
{
}

void DapCmdConnectionProblem::sendShowDialog(const QString& currentServer, const QString& errorMessage)
{
    QJsonObject jsonObject;
    jsonObject["cmd"] = "DapCmdConnectionProblem";
    jsonObject["action"] = "showDialog";
    jsonObject["currentServer"] = currentServer;
    jsonObject["errorMessage"] = errorMessage;
    
    qDebug() << "[DapCmdConnectionProblem] Sending show dialog command for server:" << currentServer;
    sendCmd(&jsonObject);
}

void DapCmdConnectionProblem::sendDialogClosed()
{
    QJsonObject jsonObject;
    jsonObject["cmd"] = "DapCmdConnectionProblem";
    jsonObject["action"] = "dialogClosed";
    
    qDebug() << "[DapCmdConnectionProblem] Sending dialog closed command";
    sendCmd(&jsonObject);
}

void DapCmdConnectionProblem::handle(const QJsonObject *rcvdObj)
{
    if (!rcvdObj) {
        qWarning() << "[DapCmdConnectionProblem] Received null object";
        return;
    }

    QString action = rcvdObj->value("action").toString();
    qDebug() << "[DapCmdConnectionProblem] Handling action:" << action;

    if (action == "showDialog") {
        handleShowDialog(*rcvdObj);
    } else if (action == "retryCurrentServer") {
        handleRetryCurrentServer(*rcvdObj);
    } else if (action == "selectNewServer") {
        handleSelectNewServer(*rcvdObj);
    } else if (action == "dialogClosed") {
        handleDialogClosed(*rcvdObj);
    } else {
        qWarning() << "[DapCmdConnectionProblem] Unknown action:" << action;
    }
}

void DapCmdConnectionProblem::handleShowDialog(const QJsonObject& obj)
{
    QString currentServer = obj.value("currentServer").toString();
    QString errorMessage = obj.value("errorMessage").toString();
    
    qDebug() << "[DapCmdConnectionProblem] Show dialog for server:" << currentServer 
             << "with error:" << errorMessage;
    
    emit sigShowConnectionProblemDialog(currentServer, errorMessage);
}

void DapCmdConnectionProblem::handleRetryCurrentServer(const QJsonObject& obj)
{
    Q_UNUSED(obj)
    qDebug() << "[DapCmdConnectionProblem] User chose to retry current server";
    emit sigRetryCurrentServer();
}

void DapCmdConnectionProblem::handleSelectNewServer(const QJsonObject& obj)
{
    Q_UNUSED(obj)
    qDebug() << "[DapCmdConnectionProblem] User chose to select new server";
    emit sigSelectNewServer();
}

void DapCmdConnectionProblem::handleDialogClosed(const QJsonObject& obj)
{
    Q_UNUSED(obj)
    qDebug() << "[DapCmdConnectionProblem] Dialog was closed";
    emit sigDialogClosed();
}

void DapCmdConnectionProblem::slotRetryCurrentServer()
{
    QJsonObject jsonObject;
    jsonObject["cmd"] = "DapCmdConnectionProblem";
    jsonObject["action"] = "retryCurrentServer";
    
    qDebug() << "[DapCmdConnectionProblem] Sending retry current server command";
    sendCmd(&jsonObject);
}

void DapCmdConnectionProblem::slotSelectNewServer()
{
    QJsonObject jsonObject;
    jsonObject["cmd"] = "DapCmdConnectionProblem";
    jsonObject["action"] = "selectNewServer";
    
    qDebug() << "[DapCmdConnectionProblem] Sending select new server command";
    sendCmd(&jsonObject);
}

void DapCmdConnectionProblem::slotDialogClosed()
{
    sendDialogClosed();
} 