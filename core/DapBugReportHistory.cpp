#include "DapBugReportHistory.h"
#include "DapDataLocal.h"

DapBugReportHistory::DapBugReportHistory(QObject *parent)
    : QObject(parent)
    ,m_bugReportsList(new QList<_BugReportInfo>)
{

}

QList<_BugReportInfo>* DapBugReportHistory::getBugReportsList(){
    return m_bugReportsList;
};

QList<QString> DapBugReportHistory::getBugReportStringList(){
    return bugReportInfoListToStringList(m_bugReportsList);
}

void DapBugReportHistory::setBugReportsList(QList<_BugReportInfo> *a_bugReportsList){
    m_bugReportsList = a_bugReportsList;
}


void DapBugReportHistory::loadHistoryBugReportData()
{
    QList<QString> m_tempHistoryDataList;
    DapDataLocal::instance()->loadFromSettings(TEXT_BUGREPORT_HISTORY, m_tempHistoryDataList);
    m_bugReportsList->clear();
    for(auto item : m_tempHistoryDataList)
        m_bugReportsList->push_back(_BugReportInfo{item, "unknown"});
    bugReportInfoListToStringList(m_bugReportsList);
}

QList<QString> DapBugReportHistory::bugReportInfoListToStringList(QList<_BugReportInfo>* a_bugReportInfoList){

    QList<QString> stringListSrverInfo;
    for (auto item : *a_bugReportInfoList){
        stringListSrverInfo.push_front(item.number);
    }
    return stringListSrverInfo;
}
