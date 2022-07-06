#include "DapCmdDataLocal.h"
#include <QFile>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdDataLocal::actionParam = "data_local";


DapCmdDataLocal::DapCmdDataLocal(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::DATA_LOCAL, parent)
    , dapDataLocalSettings (new DapDataSettingsLocal())
{
}

DapCmdDataLocal::~DapCmdDataLocal()
{

}

/**
 * @brief DapCmdConnect::sendCmdError
 * @param interfaceName
 */
void DapCmdDataLocal::sendSettings(QJsonObject& metadata)
{
    QJsonObject response;
    response[actionParam] = metadata;
    sendCmd(&response);
}

void DapCmdDataLocal::handle(const QJsonObject* params)
{
    qDebug() << "DapCmdDataLocal::handle";
    qDebug() << params->value(actionParam);
    if(params->value(actionParam) != QJsonValue::Undefined)
    {
        QJsonObject request = params->value(actionParam).toObject();
        if (request.value("get") != QJsonValue::Undefined)
        {
            qDebug() << "get";
            QJsonObject dataRequest = request.value("get").toObject();
            if (dataRequest.value("all") != QJsonValue::Undefined)
            {
                qDebug() << "metadata";
                QJsonObject metadata;
                ConfigData configData;
                configData.parseXML(":/data.xml");
                metadata["config"] = configData.toJson();
                metadata["settings"] = dapDataLocalSettings->toJson();
                metadata["status"] = "OK";
                sendSettings(metadata);
                return;
            }
        }
        if (request.value("save") != QJsonValue::Undefined)
        {
            QJsonObject metadata;
            QJsonObject saveData = request.value("save").toObject();
            foreach(const QString& key, saveData.keys())
            {
                QVariant value = saveData.value(key);
                dapDataLocalSettings->saveSetting(key, value);
                qDebug() << "save" << key << value;
            }
            metadata["status"] = "OK";
            sendSettings(metadata);
            return;
        }
        if (request.value("remove") != QJsonValue::Undefined)
        {
            QJsonObject metadata;
            QJsonArray removeData = request.value("remove").toArray();
            foreach(const QJsonValue key, removeData)
            {
                dapDataLocalSettings->removeSetting(key.toString());
                qDebug() << "remove" << key;
            }
            metadata["status"] = "OK";
            sendSettings(metadata);
            return;
        }
    }
}
