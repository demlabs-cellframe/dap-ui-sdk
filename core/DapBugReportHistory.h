#pragma once
#include <QObject>
#include <QMap>

struct DapBugReportHistoryItem
{
    int number;
    QString status;
    QString asString() const { return QString().asprintf ("%05d", number); }
};

class DapBugReportHistory : public QObject
{
    Q_OBJECT

public:
    using Callback = std::function<void()>;

    DapBugReportHistory (QObject *a_parent = nullptr);

    const QList<DapBugReportHistoryItem> &list() const;
    int size() const;
    bool isEmpty() const { return size() == 0; }
    DapBugReportHistoryItem &value (int a_id);
    const DapBugReportHistoryItem &value (int a_id) const;
    void remove (int a_id, int a_index);

    DapBugReportHistoryItem &operator[] (int a_id)               { return value (a_id); }
    const DapBugReportHistoryItem &operator[] (int a_id) const   { return value (a_id); }

    void addBugReports(int number, const DapBugReportHistoryItem& item);
    void setBugReports(const QMap<int, DapBugReportHistoryItem>& reports);
    const QMap<int, DapBugReportHistoryItem>& getBagreports() const { return m_items; }

    void bagreportListLoaded() { emit sigBugReportListLoaded(); }

    QJsonObject itemToJson (const DapBugReportHistoryItem &a_src);
    DapBugReportHistoryItem jsonToItem (const QJsonObject &a_src);

    void setSaveCallback(const Callback& handler) { m_saveCallback = handler; }
signals:
    void sigBugReportListLoaded();
    void sigUpdateReportsStatusError();
    void sigUpdateReportsStatusSuccess();
    void sigNewReport (int a_index);
    void sigRemoved (int a_index);

public slots:
    void slotUpdateReportsStatus (const QString &a_json);
    void slotNewReport (const QString &a_reportNumber);
private:
    void saveBugReport();
private:
    QMap<int, DapBugReportHistoryItem> m_items;

    Callback m_saveCallback = nullptr;
};

