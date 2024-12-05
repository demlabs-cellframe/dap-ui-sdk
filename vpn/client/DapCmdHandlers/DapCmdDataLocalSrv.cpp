#include "DapCmdDataLocalSrv.h"
#include "DapSerialKeyData.h"
#include "DapSerialKeyHistory.h"
#include "DapServiceDataLocal.h"

DapCmdDataLocalSrv::DapCmdDataLocalSrv (QObject *a_parent)
    : DapCmdServiceAbstract (DapJsonCmdType::DATA_LOCAL, a_parent)
{
    auto dataLocal = DapServiceDataLocal::instance();
    connect(dataLocal, &DapServiceDataLocal::valueServiceDataLocalUpdated, this, &DapCmdDataLocalSrv::serviceDataLocalUpdated);
}

DapCmdDataLocalSrv::~DapCmdDataLocalSrv()
{
}

void DapCmdDataLocalSrv::serviceDataLocalUpdated(const QJsonObject& object)
{
    QJsonObject jobj {
                     { "action", "serviceDataUpdate" },
                     { "data", object },
                     };
    sendCmd (&jobj);
}

void DapCmdDataLocalSrv::handle (const QJsonObject *a_params)
{
    static const char *base64sign = "base64,";
    QString action  = a_params->value ("action").toString();

    if(action == "localUpdate")
    {
        DapServiceDataLocal::instance()->fromJson(a_params->value("data").toObject());
    }
    else if(action == "migration")
    {
        DapServiceDataLocal::instance()->setMigrationInfo(a_params->value("data").toObject());
    }
    else if(action == "set")
    {
        QString name    = a_params->value ("name").toString();

        QVariant value;
        QJsonValue jsValue  = a_params->value ("value");

        /* check if string */
        if (jsValue.type() == QJsonValue::String)
        {
            QString stringValue = jsValue.toString();

            /* check if base 64 */
            if (stringValue.startsWith (base64sign))
            {
                /* parse and store bytearray */
                auto base64src  = stringValue.mid(strlen (base64sign)).toUtf8();
                auto base64data = QByteArray::fromBase64 (base64src);
                value           = std::move (base64data);
            }

            /* store string */
            else
                value = stringValue;
        }

        /* store value */
        else
            value = jsValue.toVariant();

        setValue(std::move (name), std::move (value));
    }
    else if (action == "get")
    {
        QString name = a_params->value ("name").toString();
        return getValue(std::move(name));
    }
    else if (action == "remove")
    {
        QString name = a_params->value ("name").toString();
        remove(std::move(name));
    }
    else if (action == "getAll")
    {
        getAllData();
    }
}

void DapCmdDataLocalSrv::getValue(const QString& nameValue)
{
    qDebug() << "Received request for value. Name:" << nameValue;

    auto dataLocal = DapServiceDataLocal::instance();
    QVariant value;

    if (nameValue == "serial_key")
    {
        value = dataLocal->serialKeyData()->serialKey();
        qDebug() << "Returning serial_key:" << value.toString();
    }
    else if (nameValue == "is_key_activate")
    {
        value = dataLocal->serialKeyData()->isActivated();
        qDebug() << "Returning is_key_activate:" << value.toBool();
    }
    else if (nameValue == "serial_history_key")
    {
        value = dataLocal->serialKeyHistory()->list();
        qDebug() << "Returning serial_history_key list, size:" << value.toList().size();
    }
    else if (nameValue == "auth_info_key")
    {
        value = dataLocal->serialKeyDataToJson();
        qDebug() << "Returning auth_info_key";
    }
    else
    {
        value = dataLocal->settings()->value(nameValue);
        qDebug() << "Returning value for setting:" << nameValue << ", Value:" << value;
    }

    if (!value.isValid()) {
        qWarning() << "Warning: Invalid value for name:" << nameValue;
    }

    sendValue(nameValue, value);
}

void DapCmdDataLocalSrv::setValue(const QString& nameValue, const QVariant& value)
{

    DapServiceDataLocal::instance()->saveSetting(nameValue, value);
    sendValue (nameValue, value);
}

void DapCmdDataLocalSrv::remove(const QString& nameValue)
{
    DapServiceDataLocal::instance()->removeSetting(nameValue);
    sendRemove (nameValue);
}

void DapCmdDataLocalSrv::getAllData()
{
    QJsonObject jObj  = DapServiceDataLocal::instance()->toJson();
    sendAllData (jObj);
}

void DapCmdDataLocalSrv::sendAllData(const QJsonObject &a_data)
{
    QJsonObject jobj {
                     { "action", "setAll" },
                     { "data", a_data },
                     };
    sendCmd (&jobj);
}

void DapCmdDataLocalSrv::sendRemove (const QString &a_name)
{
    QJsonObject jobj {
                     { "action", "remove" },
                     { "name", a_name },
                     };
    sendCmd (&jobj);
}

void DapCmdDataLocalSrv::sendValue (const QString &a_name, const QVariant &a_value)
{
    QJsonObject jobj
        {
         { "action", "set" },
         { "name", a_name },
         };

    if (a_value.type() == QVariant::ByteArray)
        jobj.insert ("value", QString ("base64,") + a_value.toByteArray().toBase64().constData());
    else
        jobj.insert ("value", a_value.toJsonValue());

    sendCmd (&jobj);
}
