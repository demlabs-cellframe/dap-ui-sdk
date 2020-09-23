#ifndef DAPNEWSHANDLER_H
#define DAPNEWSHANDLER_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QMap>

class DapNews : public QObject
{
    Q_OBJECT
    explicit DapNews(QObject *parent = nullptr);
public:
    using NewsRecord = QMap<QString, QString>;

    static DapNews* instance();

    QString getCurrentNewsText() const;
    QString getCurrentNewsUrl() const;
    bool isNewsAvailable() const;

public slots:
    // @brief Slot for processing the signal that the current news is marked as read
    void newsChecked();
    // @brief Slot for receive news list. In the end once emit 'haveNews' signal.
    void onGotNews(QList<QMap<QString, QString>> listNews);

signals:
    // @brief Signal to the UI that we have more news
    // if it's about version update we also emit 'haveUpdate' signal
    void haveNews(const QString &message, const QString &url);
    // @brief We have version update
    void haveUpdate(const QString &url);

private:
    // @brief just add news, no signal emit
    void addNews(const QMap<QString, QString>& record);
    struct NewsItem
    {
        QDateTime m_dateTime;
        quint64   m_id = 0;
        QString   m_type;
        QString   m_url;
        QString   m_text;
    };
    // @brief we can have only one news about update and we keep it separetly
    NewsItem m_aboutUpdate;
    //m_list is sorted by dateTime and id
    // @brief we keep the timestamp of the latest news and afterward we ignore all the older ones
    QDateTime m_lastRead;
    // @brief m_list is sorted by dateTime and id
    //oldest news in back and newest one in front
    QList<NewsItem> m_list;
};


#endif // DAPNEWSHANDLER_H
