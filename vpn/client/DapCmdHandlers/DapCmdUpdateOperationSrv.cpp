#include "DapCmdUpdateOperationSrv.h"


DapCmdUpdateOperationSrv::DapCmdUpdateOperationSrv(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::UPDATE_OPERATION, parent)
{

}

void DapCmdUpdateOperationSrv::setDownloadProgress(quint64 load, quint64 total)
{
    QJsonObject jo;
    jo["load"] = QString::number(load);
    jo["total"] = QString::number(total);
    sendCmd(&jo);
}

void DapCmdUpdateOperationSrv::handle(const QJsonObject *params)
{
    if (params->value("start_download") != QJsonValue::Undefined)
    {
        QJsonObject downloadInfo = params->value("start_download").toObject();
        emit startDownload(downloadInfo.value("url").toString(),
                           downloadInfo.value("pack").toString());
    }
    if (params->value("start_update") != QJsonValue::Undefined)
        emit startUpdate();
}
