#include "DapCmdCdbCtl.h"
#include "DapServiceDataLocal.h"

enum DapCmdCdbCtlValue
{
    GET,
    SET,
};

const QString DapCmdCdbCtl::s_fieldCdb =
    DapJsonParams::toString (DapJsonParams::CDB);

const QString DapCmdCdbCtl::s_fieldValue =
    DapJsonParams::toString (DapJsonParams::VALUE);

DapCmdCdbCtl::DapCmdCdbCtl (QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::CDB_CTL, parent)
{

}

void DapCmdCdbCtl::sendCmdGetList (const QString &a_value)
{
    static QJsonObject result = { {s_fieldValue, DapCmdCdbCtlValue::GET} };
    result[s_fieldCdb]        = a_value;
    sendCmd(&result);
}

void DapCmdCdbCtl::handle (const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
    /* get value and check */
    auto value  = params->value (s_fieldValue);
    if (value == QJsonValue::Undefined)
    {
        qWarning() << "Invalid value" << params;
        return;
    }

    /* behave based on value */
    switch (DapCmdCdbCtlValue (value.toInt()))
    {
    case DapCmdCdbCtlValue::GET:
    {
        /* collect servers into string list */
        const auto &list  = DapServiceDataLocal::instance()->cdbServersList();
        QStringList result;

        for (const auto &item : list)
            result << item.toString();

        /* join and send */
        sendCmdGetList (result.join (','));
    } break;

    case DapCmdCdbCtlValue::SET:
    {
        static const QString SETTING_CDB { "cdb" };

        /* get new cdb servers list and check */
        auto cdb  = params->value (s_fieldCdb);
        if (value == QJsonValue::Undefined)
        {
            qWarning() << "Invalid cdb list" << params;
            return;
        }

        /* get actual list and update */
        auto src  = cdb.toString();
        auto list = src.split(',');
        auto cdbs = DapCdbServerList::toServers (list);

        auto data = DapServiceDataLocal::instance();
        data->updateCdbList(cdbs);
        data->saveSetting (SETTING_CDB, src.toLatin1().toBase64());
    } break;

    };
}

/*-----------------------------------------*/
