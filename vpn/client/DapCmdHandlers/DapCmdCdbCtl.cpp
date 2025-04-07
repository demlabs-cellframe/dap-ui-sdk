#include "DapCmdCdbCtl.h"
#include "DapServiceDataLocal.h"
#include "DapCdbManager.h"
#include "dap_net.h"

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
        const auto& list = DapCdbManager::instance().servers();
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
        auto cdb = params->value(s_fieldCdb);
        if (cdb == QJsonValue::Undefined)
        {
            qWarning() << "Invalid cdb list" << params;
            return;
        }

        /* parse string into servers and update manager */
        auto src = cdb.toString();
        auto list = src.split(',', Qt::SkipEmptyParts);

        auto& manager = DapCdbManager::instance();
        manager.clear();

        for (const QString& item : list)
        {
            QByteArray str = item.toLatin1();
            char* raw = str.data();
            char l_host[257] = {};
            uint16_t port = 0;

            if (dap_net_parse_config_address(raw, l_host, &port, nullptr, nullptr))
            {
                manager.addServer(QString(l_host), port ? port : 80);
            }
        }

        DapServiceDataLocal::instance()->saveSetting(SETTING_CDB, src.toLatin1().toBase64());
    } break;

    };
}

/*-----------------------------------------*/
