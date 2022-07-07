#include "DapCmdDataLocal.h"
#include "DapDataLocal.h"


const QString DapCmdDataLocal::actionParam = "data_local";
void DapCmdDataLocal::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    if(params->contains(actionParam))
    {
        qDebug() << "Local data ready";
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
    qDebug() << "Request gui data from service";
    QJsonObject response;
    QJsonObject get;
    QJsonObject all;
    all["all"] = true;
    get["get"] = all;
    response[actionParam] = get;
    sendCmd(&response);
}

void DapCmdDataLocal::sendSaveRequest(QMap<QString, QVariant> data)
{
    foreach (auto key, data.keys())
        qDebug() << "Send save request" << key;
    QJsonObject response;
    QJsonObject save;
    save["save"] = DapDataSettings::toJson(data);
    response[actionParam] = save;
    sendCmd(&response);
}

void DapCmdDataLocal::sendRemoveRequest(QStringList keys)
{
    foreach (auto key, keys)
        qDebug() << "Send remove request" << key;
    QJsonObject response;
    QJsonObject remove;
    remove["remove"] = DapDataSettings::toJson(keys);
    response[actionParam] = remove;
    qDebug() << response;
    sendCmd(&response);
}
