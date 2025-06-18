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
    DapCmdServiceAbstract::handle(params);
    if (params->value("start_download") != QJsonValue::Undefined)
    {
        QJsonObject downloadInfo = params->value("start_download").toObject();
        emit startDownload(downloadInfo.value("url").toString(),
                           downloadInfo.value("pack").toString());
    }
    if (params->value("start_update") != QJsonValue::Undefined)
        emit startUpdate();

    if (params->value("download_remove") != QJsonValue::Undefined)
    {
        QJsonObject downloadRemoveInfo = params->value("download_remove").toObject();
        emit removeDownload(downloadRemoveInfo.value("pack").toString());
    }
}
