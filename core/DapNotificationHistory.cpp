#include "DapNotificationHistory.h"
#include "DapDataLocal.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

//#define TEST_ITEMS

#define ROLEFIELD(a_name) { int (DapNotificationHistory::FieldId::a_name), #a_name }

static const QHash<int, QByteArray> s_roles =
{
  ROLEFIELD (created),
  ROLEFIELD (message),
  ROLEFIELD (type),
  ROLEFIELD (isTitle),
  ROLEFIELD (titleDate),
  ROLEFIELD (typeString),
  ROLEFIELD (createdTime),
};

static const QStringList s_months =
{
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
};

DapNotificationHistory::DapNotificationHistory (QObject *parent)
  : QAbstractListModel (parent)
{
#ifndef TEST_ITEMS
  load();
#else // TEST_ITEMS
  if (m_list.isEmpty())
  {
    append ("Can't find handle",          DapNotification::Type::Error);
    append ("No requirements met, No requirements met, No requirements met",        DapNotification::Type::Notification);
    append ("Can't parse\nprovided\ndata\n{data}\nCan't parse\nprovided\ndata\n{data}",  DapNotification::Type::Warning);
  }
#endif // TEST_ITEMS
}

DapNotificationHistory::~DapNotificationHistory()
{

}

DapNotificationHistory *DapNotificationHistory::instance()
{
  static DapNotificationHistory *i = nullptr;

  if (i == nullptr)
    {
      i = new DapNotificationHistory;
      connect (i, &QObject::destroyed,
               [ = ] { i = nullptr; });
    }

  return i;
}

void DapNotificationHistory::append (const QString &a_message, DapNotification::Type a_type)
{
  /* variables */
  QDate topDate = (m_list.isEmpty()) ? QDate() : m_list.first().titleDate;
  auto todayDt  = QDateTime::currentDateTime();
  QDate today   = todayDt.date();

  /* lambda's */
  auto insertDate = [&]
  {
    beginInsertRows (QModelIndex(), 0, 0);
    m_list.prepend (Item { true, today, DapNotification() });
    endInsertRows();
  };

  /* check if last error is the same */
  if (m_list.size() > 1)
  {
    /* get values */
    auto &item  = m_list.at (1).notification;

    auto &mesg  = item.message();
    QDate date  = item.created().date();
    QTime time[2] =
    {
      item.created().time(),
      todayDt.time(),
    };

    /* remove seconds */
    time[0].setHMS (time[0].hour(), time[0].minute(), 0, 0);
    time[1].setHMS (time[1].hour(), time[1].minute(), 0, 0);

    /* compare */
    if (time[0] == time[1]
        && date == today
        && mesg == a_message)
      return;
  }

  /* insert new top date */
  if (!topDate.isValid())
    insertDate();
  else
    if (topDate != today)
      insertDate();

  beginInsertRows (QModelIndex(), 1, 1);

  DapNotification notification
  {
    todayDt,
    a_message,
    a_type,
  };

  Item item
  {
    false,
    QDate(),
    std::move (notification)
  };

  m_list.insert (1, std::move (item));

  _delayedSave();
}

void DapNotificationHistory::clear()
{
  beginResetModel();
  m_list.clear();
  endResetModel();

  _delayedSave();
}

const QHash<int, QByteArray> &DapNotificationHistory::fields()
{
  return s_roles;
}

void DapNotificationHistory::load()
{
  beginResetModel();
  m_list.clear();

  /* variables */
  QDate threeDaysAgo = QDate::currentDate().addDays (-3);
  QByteArray source;
  QJsonArray jarr;

  /* get data */
  DapDataLocal::instance()->loadFromSettings (DapDataLocal::NOTIFICATION_HISTORY, source);
  jarr  = QJsonDocument::fromJson (source).array();

  /* prepare parsing */
  m_list.clear();

  /* parse json array */
  for (const auto &jitem : qAsConst (jarr))
    {
      /* parse */
      QJsonObject jobj              = jitem.toObject();
      DapNotification notification  = DapNotification::fromJson (jobj);
      QString dateString            = jobj.value ("date").toString();
      QDate date                    = QDate::fromString (dateString, "dd.MM.yyyy");
      Item item
      {
        jobj.value ("isTitle").toBool(),
        date,
        std::move (notification)
      };

      /* skip old */
      if (item.isTitle)
      {
        if (item.titleDate < threeDaysAgo)
          continue;
      }
      else
      {
        if (item.notification.created().date() < threeDaysAgo)
          continue;
      }

      /* store */
      m_list.append (std::move (item));
    }

  endResetModel();
}

void DapNotificationHistory::save()
{
  _removeOutdated();
  _delayedSave();
}

void DapNotificationHistory::removeOutdated()
{
  _removeOutdated (true);
}

void DapNotificationHistory::_removeOutdated (bool a_resetModel)
{
  /* variables */
  QDate threeDaysAgo = QDate::currentDate().addDays (-3);
  QVector<Item>::iterator it;

  /* start removing old items */
  if (a_resetModel)
    beginResetModel();

  /* remove all old items */
  it  = m_list.begin();
  while (it != m_list.end())
  {
    /* ------------------------------------- */
    /* title */
    /* ------------------------------------- */

    if (it->isTitle)
    {
      /* remove old */
      if (it->titleDate < threeDaysAgo)
        it  = m_list.erase (it);

      /* proceed */
      else
        it++;
    }

    /* ------------------------------------- */
    /* item */
    /* ------------------------------------- */

    else
    {
      /* remove old */
      if (it->notification.created().date() < threeDaysAgo)
        it  = m_list.erase (it);

      /* proceed */
      else
        it++;
    }

    /* ------------------------------------- */
  }

  /* finish removind items */
  if (a_resetModel)
    endResetModel();

//  /* variables */
//  QDateTime threeDaysAgo = QDate::currentDateTime().addDays (-3);
//  QVector<Item>::iterator it, titleIt = m_list.end();

//  /* start removing old items */
//  if (a_resetModel)
//    beginResetModel();

//  /* ------------------------------------- */
//  /* lambda's */
//  /* ------------------------------------- */

//  auto removeTitleIt = [&]() -> bool
//  {
//    /* if previous item was title too */
//    if (titleIt != m_list.end())
//      {
//        /* remove title itereator and invalidate iterator */
//        it      = m_list.erase (titleIt);
//        titleIt = m_list.end();
//        return true;
//      }

//    return false;
//  };

//  /* ------------------------------------- */
//  /* remove old notifications */
//  /* ------------------------------------- */

//  it  = m_list.begin();
//  while (it != m_list.end())
//    {
//      /* skip title items */
//      if (it->isTitle)
//        continue;

//      /* get notification */
//      const DapNotification &notification = it->notification;

//      /* erase, if too old */
//      if (notification.created() < threeDaysAgo)
//        it = m_list.erase (it);
//      else
//        it++;
//    }

//  /* ------------------------------------- */
//  /* remove empty title */
//  /* ------------------------------------- */

//  it  = m_list.begin();
//  while (it != m_list.end())
//    {
//      /* act on title */
//      if (it->isTitle)
//        {
//          /* if previous item was title too */
//          if (removeTitleIt())
//            continue;

//          /* store next title */
//          titleIt = it;
//          it++;
//          continue;
//        }

//      /* if item, invalidate titleIt */
//      titleIt = m_list.end();
//      it++;
//    }

//  /* ------------------------------------- */
//  /* repeat for final iterator */
//  /* ------------------------------------- */

//  removeTitleIt();

//  /* ------------------------------------- */

//  /* finish removind items */
//  if (a_resetModel)
//    endResetModel();
}

void DapNotificationHistory::_delayedSave()
{
  static QTimer *s_delay  = nullptr;

  if (s_delay == nullptr)
    {
      s_delay = new QTimer;
      s_delay->setSingleShot (true);
      s_delay->setInterval (2000);

      connect (s_delay, &QTimer::timeout,
               this, [ = ]
      {
        QJsonArray jarr;
        
        // Apply filters before saving: remove old entries and limit count
        QDate threeDaysAgo = QDate::currentDate().addDays(-3);
        int notificationCount = 0;
        const int MAX_NOTIFICATIONS = 50;

        /* collect json items with filtering */
        for (auto &item : m_list)
          {
            // Check age limits
            bool includeItem = false;
            
            if (item.isTitle)
            {
              // For title entries, check titleDate
              if (item.titleDate.isValid() && item.titleDate >= threeDaysAgo)
              {
                includeItem = true;
              }
            }
            else
            {
              // For notification entries, check created date
              if (item.notification.created().isValid() && 
                  item.notification.created().date() >= threeDaysAgo)
              {
                includeItem = true;
              }
            }
            
            // Skip if too old or if we've reached the limit
            if (!includeItem || notificationCount >= MAX_NOTIFICATIONS)
            {
              continue;
            }
            
            /* get notification fields */
            QJsonObject jobj = item.notification.toJson();

            /* get item title fields */
            jobj.insert ("isTitle", item.isTitle);
            jobj.insert ("date",
                         (item.titleDate.isValid())
                         ? item.titleDate.toString ("dd.MM.yyyy")
                         : QString());

            /* store result */
            jarr.append (jobj);
            notificationCount++;
          }

        /* store result json array */
        QByteArray result = QJsonDocument (jarr).toJson (QJsonDocument::JsonFormat::Compact);
        
        // Check size and truncate further if needed
        const int MAX_NOTIFICATION_JSON_SIZE = 8000; // Leave room for other data
        if (result.size() > MAX_NOTIFICATION_JSON_SIZE)
        {
          qWarning() << "[DapNotificationHistory][_delayedSave] Notification JSON too large (" 
                     << result.size() << " bytes), truncating further";
          
          // Remove entries from the end until size is acceptable
          while (jarr.size() > 0 && result.size() > MAX_NOTIFICATION_JSON_SIZE)
          {
            jarr.removeLast();
            result = QJsonDocument(jarr).toJson(QJsonDocument::JsonFormat::Compact);
          }
          
          qDebug() << "[DapNotificationHistory][_delayedSave] Notification history truncated to" 
                   << jarr.size() << "entries, size:" << result.size() << "bytes";
        }
        
        DapDataLocal::instance()->saveToSettings (DapDataLocal::NOTIFICATION_HISTORY, result);
        
        qDebug() << "[DapNotificationHistory][_delayedSave] Saved" << jarr.size() 
                 << "notifications, JSON size:" << result.size() << "bytes";
      });
    }

  s_delay->start();
}

QHash<int, QByteArray> DapNotificationHistory::roleNames() const
{
  return fields();
}

int DapNotificationHistory::rowCount (const QModelIndex &) const
{
  return m_list.size();
}

QVariant DapNotificationHistory::data (const QModelIndex &index, int role) const
{
  if (!index.isValid()
      || index.row() >= m_list.size())
    return QVariant();

  static auto typeName = [] (DapNotification::Type a_type) -> QString
  {
    switch (a_type)
      {
      case DapNotification::Type::Error:        return tr ("Error");
      case DapNotification::Type::Notification: return tr ("Notification");
      case DapNotification::Type::Warning:      return tr ("Warning");
      }
    return QString();
  };

  auto fid    = DapNotification::FieldId (role);
  auto &item  = m_list.at (index.row());
  auto result = item.notification.value (fid);

  if (result.isNull())
    {
      switch (DapNotificationHistory::FieldId (fid))
        {
        case FieldId::isTitle:      return item.isTitle;
        case FieldId::titleDate:
        {
          if (!item.isTitle)
            return QString();

          QString month = s_months.value (item.titleDate.month()-1, QString());
          if (month.isEmpty())
            return item.titleDate.toString ("MMMM dd");

          return month + " " + item.titleDate.toString ("dd");
        }
        case FieldId::typeString:   return typeName (item.notification.type());
        case FieldId::createdTime:  return item.notification.createdStringAM();
        }
    }

  return result;
}

