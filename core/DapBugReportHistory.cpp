
#include "DapBugReportHistory.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

static const char* STATUS_UNKNOWN  = "unknown";

QJsonObject DapBugReportHistory::itemToJson (const DapBugReportHistoryItem &a_src)
{
    return QJsonObject
        {
         {"number", a_src.number},
         {"status", a_src.status},
         };
}

DapBugReportHistoryItem DapBugReportHistory::jsonToItem (const QJsonObject &a_src)
{
    return DapBugReportHistoryItem
        {
            a_src.value ("number").toInt(),
            a_src.value ("status").toString()
        };
}

DapBugReportHistory::DapBugReportHistory (QObject *a_parent)
    : QObject (a_parent)
{

}

const QList<DapBugReportHistoryItem> &DapBugReportHistory::list() const
{
    static QList<DapBugReportHistoryItem> result;
    result  = m_items.values();
    return result;
}

int DapBugReportHistory::size() const
{
    return m_items.size();
}

DapBugReportHistoryItem &DapBugReportHistory::value (int a_id)
{
    static DapBugReportHistoryItem dummy;

    if (!m_items.contains(a_id))
        return dummy = DapBugReportHistoryItem();

    return m_items[a_id];
}

const DapBugReportHistoryItem &DapBugReportHistory::value (int a_id) const
{
    return ((DapBugReportHistory*)this)->value (a_id);
}

void DapBugReportHistory::remove (int a_id, int a_index)
{
    m_items.remove (a_id);
    emit sigRemoved (a_index);
    saveBugReport();
};

void DapBugReportHistory::addBugReports(int number, const DapBugReportHistoryItem& item)
{
    m_items.insert(number, item);
}

void DapBugReportHistory::setBugReports(const QMap<int, DapBugReportHistoryItem> &reports)
{
    m_items.clear();
    m_items = reports;
    emit sigBugReportListLoaded();
}

void DapBugReportHistory::slotUpdateReportsStatus (const QString &a_json)
{
    if (a_json.isEmpty())
    {
        qDebug() << "Empty bugreport status answer";
        return;
    }

    auto bugReportArray = QJsonDocument::fromJson (a_json.toUtf8()).array();

    if (!bugReportArray.first().toObject().contains ("error"))
    {
        for (const auto &item : qAsConst(bugReportArray))
        {
            auto jsItem     = item.toObject();
            auto bugreport  = jsItem.value ("bugreport").toString().toInt();
            auto status     = jsItem.value ("current_status").toString();
            if (bugreport)
                m_items.insert (bugreport, { bugreport, status.replace ('_', ' ') });
        }

        saveBugReport();

        emit sigUpdateReportsStatusSuccess();
    }
    else
        emit sigUpdateReportsStatusError();
}

void DapBugReportHistory::slotNewReport (const QString &a_reportNumber)
{
    int number  = a_reportNumber.toInt();

    if (!number)
        return;

    /* check if already exists */
    for (const auto &item : qAsConst(m_items))
        if (QString::number(item.number) == a_reportNumber)
            return;

    /* store and save */
    m_items.insert (number, { number, STATUS_UNKNOWN });
    saveBugReport();

    /* notify */
    emit sigNewReport (m_items.size() - 1);
}

void DapBugReportHistory::saveBugReport()
{
    if(m_saveCallback)
    {
        m_saveCallback();
    }
    else
    {
        qWarning() << "[DapBugReportHistory][saveBugReport] No save callback is assigned.";
    }
}
