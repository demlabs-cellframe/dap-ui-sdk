/* INCLUDES */
#include "DapBugReportHistory.h"
#include "DapDataLocal.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

/* DEFS */
static const char* STATUS_UNKNOWN  = "unknown";

/* LINKS */
inline QJsonObject itemToJson (const DapBugReportHistoryItem &a_src)
{
  return QJsonObject
  {
    {"number", a_src.number},
    {"status", a_src.status},
  };
}

inline DapBugReportHistoryItem jsonToItem (const QJsonObject &a_src)
{
  return DapBugReportHistoryItem
  {
    a_src.value ("number").toInt(),
    a_src.value ("status").toString()
  };
}

inline void legacyLoad (QMap<int, DapBugReportHistoryItem> &a_items)
{
  /* vars */
  QList<QString> list;

  /* get array from settings */
  DapDataLocal::instance()->loadFromSettings (TEXT_BUGREPORT_HISTORY, list);

  /* parse in cycle */
  a_items.clear();
  for (const auto &number : qAsConst(list))
    {
      auto item = DapBugReportHistoryItem { number.toInt(), STATUS_UNKNOWN };
      if (item.number)
        a_items.insert (item.number, std::move (item));
    }
}

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapBugReportHistory::DapBugReportHistory (QObject *a_parent)
  : QObject (a_parent)
{

}

/********************************************
 * METHODS
 *******************************************/

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
  sigRemoved (a_index);
  save();
};

void DapBugReportHistory::load()
{
  /* do not load list again */
  if (!m_items.isEmpty())
    return emit sigBugReportListLoaded();

  /* vars */
  QJsonArray array;

  /* get array from settings */
  if (DapDataLocal::instance()->loadFromSettings (TEXT_BUGREPORT_HISTORY, array)
      && !array.isEmpty())
    {
      /* parse array */
      m_items.clear();
      for (const auto &obj : qAsConst (array))
        {
          auto item = jsonToItem (obj.toObject());
          if (item.number)
            m_items.insert (item.number, std::move (item));
        }
    }
  /* or do legacy load instead */
  else
    return legacyLoad (m_items);

  /* notify */
  emit sigBugReportListLoaded();

  /* clear memory */
  while (array.size())
    array.takeAt (array.size() - 1);
}

void DapBugReportHistory::save()
{
  /* vars */
  QJsonArray array;

  /* fill in cycle */
  for (const auto &item : qAsConst (m_items))
    array << itemToJson (item);

  /* store into settings */
  DapDataLocal::instance()->saveToSettings (TEXT_BUGREPORT_HISTORY, array);

  /* clear memory */
  while (array.size())
    array.takeAt (array.size() - 1);
}

/********************************************
 * SLOTS
 *******************************************/

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

      save();

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
    if (item.number == number)
      return;

  /* store and save */
  m_items.insert (number, { number, STATUS_UNKNOWN });
  save();

  /* notify */
  emit sigNewReport (m_items.size() - 1);
}

/*-----------------------------------------*/
