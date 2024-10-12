/* INCLUDES */

#include "DapCmdDataLocalSrv.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdDataLocalSrv::DapCmdDataLocalSrv (QObject *a_parent)
    : DapCmdServiceAbstract (DapJsonCmdType::DATA_LOCAL, a_parent)
{

}

DapCmdDataLocalSrv::~DapCmdDataLocalSrv()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdDataLocalSrv::handle (const QJsonObject *a_params)
{
    static const char *base64sign = "base64,";
    QString action  = a_params->value ("action").toString();

    /*-----------------------------------------*/

    if (action == "set")
    {
        QString name    = a_params->value ("name").toString();
        int mid         = a_params->value ("mid").toInt();

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
                auto base64src  = stringValue.mid (strlen (base64sign)).toUtf8();
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

        //QByteArray data = QByteArray::fromBase64 (a_params->value ("value").toString().toUtf8());
        //return emit sigSetValue (std::move (name), std::move (data), mid);
        return emit sigSetValue (std::move (name), std::move (value), mid);
    }

    /*-----------------------------------------*/

    if (action == "get")
    {
        QString name    = a_params->value ("name").toString();
        int mid         = a_params->value ("mid").toInt();
        return emit sigGetValue (std::move (name), mid);
    }

    /*-----------------------------------------*/

    if (action == "remove")
    {
        QString name    = a_params->value ("name").toString();
        int mid         = a_params->value ("mid").toInt();
        return emit sigRemoveValue (std::move (name), mid);
    }

    /*-----------------------------------------*/

    if (action == "getAll")
    {
        QString clType  = a_params->value ("clientType").toString();
        int mid         = a_params->value ("mid").toInt();
        return emit sigGetAllData (mid, std::move (clType));
    }

    /*-----------------------------------------*/

    if (action == "oldConfig")
    {
        QString oldFilename = a_params->value ("filename").toString();
        emit sigOldConfigFilename (std::move (oldFilename));
    }

    /*-----------------------------------------*/
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdDataLocalSrv::sendAllData (const QJsonObject &a_data, const int a_msgId, const QString &a_clientType)
{
    QJsonObject jobj {
                     { "action", "setAll" },
                     { "clientType", a_clientType },
                     { "data", a_data },
                     { "mid", a_msgId },
                     };
    sendCmd (&jobj);
}

void DapCmdDataLocalSrv::sendRemove (const QString &a_name, const int a_msgId)
{
    QJsonObject jobj {
                     { "action", "remove" },
                     { "name", a_name },
                     { "mid", a_msgId },
                     };
    sendCmd (&jobj);
}

void DapCmdDataLocalSrv::sendValue (const QString &a_name, const QVariant &a_value, const int a_msgId)
{
    QJsonObject jobj
        {
         { "action", "set" },
         { "name", a_name },
         { "mid", a_msgId },
         //{ "value", a_data.toBase64().constData() },
         };

    if (a_value.type() == QVariant::ByteArray)
        jobj.insert ("value", QString ("base64,") + a_value.toByteArray().toBase64().constData());
    else
        jobj.insert ("value", a_value.toJsonValue());

    sendCmd (&jobj);
}

/*-----------------------------------------*/
