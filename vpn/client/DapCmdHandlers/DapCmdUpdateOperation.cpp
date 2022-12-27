#include "DapCmdUpdateOperation.h"


DapCmdUpdateOperation::DapCmdUpdateOperation(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::UPDATE_OPERATION, parent)
{

}

void DapCmdUpdateOperation::setDownloadProgress(quint64 load, quint64 total)
{
    QJsonObject jo;
    jo["load"] = QString::number(load);
    jo["total"] = QString::number(total);
    sendCmd(&jo);
}

void DapCmdUpdateOperation::handle(const QJsonObject *params)
{
    if (params->value("start_download") != QJsonValue::Undefined)
        emit startDownload(params->value("start_download").toString());
    if (params->value("start_update") != QJsonValue::Undefined)
    {
        qDebug() << "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
        emit startUpdate();
    }
}
