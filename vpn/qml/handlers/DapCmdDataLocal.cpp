#include "DapCmdDataLocal.h"

#define DEBUGINFO qDebug() << " --->UiCMD<--- "

DapCmdDataLocal::DapCmdDataLocal (QObject *a_parent)
    : DapCmdClientAbstract (DapJsonCmdType::DATA_LOCAL, a_parent)
{
}

DapCmdDataLocal::~DapCmdDataLocal()
{
}

void DapCmdDataLocal::handleResult (const QJsonObject &a_params)
{
    static const char *base64sign = "base64,";
    QString action  = a_params.value ("action").toString();

    if (action == "setAll")
    {
        if(m_isAllData)
        {
            return;
        }
        m_isAllData = true;
        QJsonObject object  = a_params.value ("data").toObject();
        object.insert("action", "setAll");
        emit newDataSignal(object);
    }
    else if(action == "serviceDataUpdate")
    {
        emit newDataSignal(a_params.value ("data").toObject());
    }
    else if(action == "set")
    {
        QString name    = a_params.value ("name").toString();
        int mid         = a_params.value ("mid").toInt();

        QVariant value;
        QJsonValue jsValue  = a_params.value ("value");

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

        emit sigGotValue (std::move (name), std::move (value), mid);
    }
    else if (action == "remove")
    {
        QString name    = a_params.value ("name").toString();
        int mid         = a_params.value ("mid").toInt();
        emit sigGotRemove(std::move(name), mid);
    }
}

void DapCmdDataLocal::handleError (int code, const QString &message)
{
    DEBUGINFO << __PRETTY_FUNCTION__ << QString ("%1 %2").arg (code).arg (message);
}

void DapCmdDataLocal::requestValue (const QString &a_name)
{
    QJsonObject jobj {
                     { "action", "get" },
                     { "name", a_name },
                     };
    sendCmd(&jobj);
}

void DapCmdDataLocal::requestAllData()
{
    QJsonObject jobj {
                     { "action", "getAll" },
                     };
    sendCmd(&jobj);
}

void DapCmdDataLocal::sendValue(const QString &a_name, const QVariant &a_value)
{
    QJsonObject jobj {
                     { "action", "set" },
                     { "name", a_name },
                     };

    if (a_value.type() == QVariant::ByteArray)
        jobj.insert ("value", QString ("base64,") + a_value.toByteArray().toBase64().constData());
    else
        jobj.insert ("value", a_value.toJsonValue());

    sendCmd(&jobj);
}

void DapCmdDataLocal::sendRemove(const QString &a_name)
{
    QJsonObject jobj {
                     { "action", "remove" },
                     { "name", a_name },
                     };
    sendCmd(&jobj);
}

void DapCmdDataLocal::sendMigrate(const QJsonObject& object)
{
    QJsonObject jobj {
                     { "action", "migration" },
                     { "data", object },
                     };
    sendCmd(&jobj);
}

void DapCmdDataLocal::dataLocalUpdated(const QJsonObject& object)
{
    QJsonObject jobj {
                     { "action", "localUpdate" },
                     { "data", object },
                     };
    sendCmd(&jobj);
}
