#include "DapBugReportHistory.h"
#include "DapDataLocal.h"

DapBugReportHistory::DapBugReportHistory(QObject *parent)
    : QObject(parent)
    ,m_bugReportsList(new QList<_BugReportInfo>)
{

}

QList<_BugReportInfo> DapBugReportHistory::getBugReportsList(){
    getHistoryBugReportData();
    return *m_bugReportsList;
};


void DapBugReportHistory::getHistoryBugReportData()
{
    QList<QString>* m_tempHistoryDataList = new QList<QString>;
    DapDataLocal::instance()->loadFromSettings(TEXT_BUGREPORT_HISTORY, *m_tempHistoryDataList);
    for(auto item : *m_tempHistoryDataList)
        m_bugReportsList->push_front(_BugReportInfo{item, "unknown"});
    bugReportInfoListToStringList(*m_bugReportsList);
}

void DapBugReportHistory::bugReportInfoListToStringList(QList<_BugReportInfo> a_bugReportInfoList){

    QList<QString> stringListSrverInfo;
    for (auto item : a_bugReportInfoList){
        stringListSrverInfo.push_front(item.number);
    }
    emit bugReportListLoaded(stringListSrverInfo);
}
