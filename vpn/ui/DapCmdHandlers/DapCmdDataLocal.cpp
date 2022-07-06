#include "DapCmdDataLocal.h"
#include "DapDataLocal.h"


const QString DapCmdDataLocal::actionParam = "data_local";
void DapCmdDataLocal::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    if(params->contains(actionParam))
    {
        qDebug() << "Local data ready";
//        qDebug() << params->value(actionParam);
        QJsonObject jData = params->value(actionParam).toObject();
        if (jData.value("config") != QJsonValue::Undefined)
        {
            QJsonObject jobj = jData.value("config").toObject();
            DapDataLocal::instance()->configFromJson(&jobj);
        }
        if (jData.value("settings") != QJsonValue::Undefined)
        {
            QJsonObject jobj = jData.value("settings").toObject();
            DapDataLocal::instance()->settingsFromJson(&jobj);
        }
        emit dataReceived();
    }
}

void DapCmdDataLocal::allDataRequest()
{
    qDebug() << "Request local data from service";
    QJsonObject response;
    QJsonObject get;
    QJsonObject all;
    all["all"] = true;
    get["get"] = all;
    response[actionParam] = get;
    qDebug() << response;
    sendCmd(&response);
}

void DapCmdDataLocal::sendSaveRequest(QMap<QString, QVariant> data)
{
    foreach (auto key, data.keys())
        qDebug() << "send save request" << key << data[key];
    QJsonObject response;
    QJsonObject save;
    save["save"] = DapDataSettings::toJson(data);
    response[actionParam] = save;
    qDebug() << response;
    sendCmd(&response);
}

void DapCmdDataLocal::sendRemoveRequest(QStringList keys)
{
    foreach (auto key, keys)
        qDebug() << "send remove request" << key;
    QJsonObject response;
    QJsonObject remove;
    remove["remove"] = DapDataSettings::toJson(keys);
    response[actionParam] = remove;
    qDebug() << response;
    sendCmd(&response);
}
