#include <QMap>
#include <QDateTime>
#include "DapNews.h"

DapNews::DapNews(QObject *parent) : QObject(parent)
{
}

DapNews *DapNews::instance()
{
    static DapNews s_instance;
    return &s_instance;
}

void DapNews::addNews(const QMap<QString, QString> &record)
{
    NewsItem item;
    item.m_dateTime.setTime_t(record.value("timestamp").toLongLong());
    item.m_id = record.value("id").toULongLong();
    item.m_type = record.value("type");
    item.m_url = record.value("url");

    QString message{record.value("message")};
    message.replace("PRODUCT_NAME", DAP_BRAND);
    message.replace("COMPANY_NAME", DAP_ORGANIZATION_NAME);
    item.m_text = message;

    if (item.m_type == "update_version") {
        m_aboutUpdate = item;
    }
    else if (item.m_dateTime > m_lastRead) {
        m_list.insert(std::lower_bound(m_list.begin(), m_list.end(), item, [](const NewsItem& lhs, const NewsItem& rhs)->bool {
            return lhs.m_dateTime < rhs.m_dateTime;
        }), item);
    }
    // else news is old and already read - ignore it
}

QString DapNews::getCurrentNewsText() const
{
    if (m_aboutUpdate.m_url.isEmpty())
        return tr("New update available");
    else if (!m_list.isEmpty())
        return m_list.back().m_text;
    else
        return QString();
}
QString DapNews::getCurrentNewsUrl() const
{
    if (!m_aboutUpdate.m_url.isEmpty())
        return m_aboutUpdate.m_url;
    else if (!m_list.isEmpty())
        return m_list.back().m_url;
    else
        return QString();
}

bool DapNews::isNewsAvailable() const
{
    return !m_aboutUpdate.m_url.isEmpty() || (!m_list.isEmpty() && !m_list.last().m_text.isEmpty());
}

void DapNews::newsChecked()
{
    m_lastRead = m_list.back().m_dateTime;
    m_list.pop_back();
}

void DapNews::onGotNews(QList<QMap<QString, QString> > listNews)
{
    for (auto item : listNews){
        if(item.value("type") == "update_version") {
            QStringList version = item.value("version").split(QRegExp("[.,-]"));
            QStringList current = QString("6.0-0").split(QRegExp("[.,-]"));

            int verMajNew = version.size() > 0? version.at(0).toInt(): 0;
            int verMajActual = current.size() > 0? current.at(0).toInt(): 0;

            int verMinNew    = version.size() > 1? version.at(1).toInt(): 0;
            int verMinActual = current.size() > 1? current.at(1).toInt(): 0;

            int verPatNew    = version.size() > 2? version.at(2).toInt(): 0;
            int verPatActual = current.size() > 2? current.at(2).toInt(): 0;

            bool keyStartNews = false;

            if(verMajNew > verMajActual) keyStartNews = true;
            if(verMinNew > verMinActual && verMajNew == verMajActual) keyStartNews = true;
            if(verPatNew > verPatActual && verMajNew == verMajActual && verMinNew == verMinActual) keyStartNews = true;

            if (keyStartNews)
            {
                addNews(item);
            }
        }
        else
            addNews(item);
    }

    if (isNewsAvailable()) {
        emit haveNews(getCurrentNewsText(), getCurrentNewsUrl());
        if (!m_aboutUpdate.m_url.isEmpty())
            emit haveUpdate(getCurrentNewsUrl());
    }
}
