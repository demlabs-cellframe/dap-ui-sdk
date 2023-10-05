/* INCLUDES */

#include "DapNotificationHistory.h"
#include "DapDataLocal.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

/* DEFS */

//#define TEST_ITEMS

#define ROLEFIELD(a_name) { int (DapNotificationHistory::FieldId::a_name), #a_name }

//class SortedIntList : public QList<int>
//{
//public:
//  void addSorted (int a_value)
//  {
//    auto it = begin();

//    while (it != end() && *it < a_value)
//      ++it;

//    if (*it == a_value)
//      return;

//    insert (it, a_value);
//  }
//};

/* VARS */

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

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

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

/********************************************
 * METHODS
 *******************************************/

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

  /* insert new top date */
  if (!topDate.isValid())
    insertDate();
  else
    if (topDate != today)
      insertDate();

  /* insert item */

  /* ------------------------------------- */
  beginInsertRows (QModelIndex(), 1, 1);
  /* ------------------------------------- */

  DapNotification notification
  {
    QDateTime::currentDateTime(),
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

  /* ------------------------------------- */
  endInsertRows();
  /* ------------------------------------- */

//  /* variables */
//  int index       = m_list.size();
//  QDate lastItem  = (m_list.isEmpty()) ? QDate() : m_list.last().notification.created().date();
//  auto todayDT    = QDateTime::currentDateTime();
//  QDate today     = todayDT.date();

//  /* lambda's */
//  auto insertDate = [&]
//  {
//    beginInsertRows (QModelIndex(), index, index);
//    m_list.append (Item { true, today, DapNotification() });
//    endInsertRows();
//    index++;
//  };

//  /* insert title */
//  if (!lastItem.isValid())
//    insertDate();
//  else if (lastItem != today)
//    insertDate();

//  /* insert item */

//  /* ------------------------------------- */
//  beginInsertRows (QModelIndex(), index, index);
//  /* ------------------------------------- */

//  DapNotification notification
//  {
//    QDateTime::currentDateTime(),
//    a_message,
//    a_type,
//  };

//  Item item
//  {
//    false,
//    QDate(),
//    std::move (notification)
//  };

//  m_list.append (std::move (item));

//  /* ------------------------------------- */
//  endInsertRows();
//  /* ------------------------------------- */

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
  DapDataLocal::instance()->loadFromSettings (NOTIFICATION_HISTORY, source);
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

        /* collect json items */
        for (auto &item : m_list)
          {
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
          }

        /* store result json array */
        QByteArray result = QJsonDocument (jarr).toJson (QJsonDocument::JsonFormat::Compact);
        DapDataLocal::instance()->saveToSettings (NOTIFICATION_HISTORY, result);
      });
    }

  s_delay->start();
}

/********************************************
 * OVERRIDE
 *******************************************/

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
        case FieldId::titleDate:    return item.titleDate.toString ("MMMM dd");
        case FieldId::typeString:   return typeName (item.notification.type());
        case FieldId::createdTime:  return item.notification.createdStringAM();
        }
    }

  return result;
}

/*-----------------------------------------*/
