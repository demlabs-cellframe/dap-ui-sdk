#pragma once
#include <QObject>

class DapSerialKeyHistory : public QObject
{
    Q_OBJECT

private:
    class DapSerialKeyHistoryData;

public:
    explicit DapSerialKeyHistory (QObject *parent = nullptr);
    ~DapSerialKeyHistory();

    const QStringList &list() const;
    void appendKey(const QString& key);
    QStringList getKeysHistory();
    int size() const;
    QString value (int a_index) const;
    void setKeyList(const QStringList& list);

    QString operator[] (int a_index) const { return value (a_index); }
signals:
    void sigNewKey();

private:
    DapSerialKeyHistoryData *m_keysHistory = nullptr;

};

