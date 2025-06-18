/* INCLUDES */
#include "dapqmlmodelbugreports.h"
#include <DapDataLocal.h>

/* DEFS */
enum Column
{
  C_NUMBER,
  C_STATUS,
};

/* VARS */
static DapQmlModelBugReports *__inst = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelBugReports::DapQmlModelBugReports(QObject *parent)
  : QAbstractTableModel{parent}
{
  /* vars */
  __inst  = this;

  /* signals */
  auto history  = DapDataLocal::instance()->bugReportHistory();

  connect (history, &DapBugReportHistory::sigNewReport,
           this, &DapQmlModelBugReports::_slotNewReport);
  connect (history, &DapBugReportHistory::sigRemoved,
           this, &DapQmlModelBugReports::_slotRemoved);
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelBugReports *DapQmlModelBugReports::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelBugReports;
  return __inst;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelBugReports::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return DapDataLocal::instance()->bugReportHistory()->size();
}

int DapQmlModelBugReports::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlModelBugReports::data(const QModelIndex &index, int role) const
{
  /* check index */
  if (!index.isValid())
    return QVariant();

  /* check boundaries */
  const auto &list  = DapDataLocal::instance()->bugReportHistory()->list();
  int size          = list.size();
  if (index.row() >= size)
    return QVariant();

  /* variables */
  auto column = Column (role);
  const auto &item   = list[size - index.row() - 1];

  /* return value by type */
  switch (column)
    {
    case C_NUMBER:  return "Report #" + item.asString();
    case C_STATUS:  return item.status;
    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelBugReports::roleNames() const
{
  static QHash<int, QByteArray> names;

  if (names.isEmpty())
    {
      names.insert (0, "name");
      names.insert (1, "state");
    }

  return names;
}

QVariant DapQmlModelBugReports::value (int a_index, const QString &a_name) const
{
  auto roles  = roleNames();
  int id      = roles.key (a_name.toUtf8());

  if (id == -1)
    return QVariant();

  return data (index (a_index, 0), id);
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelBugReports::slotSetup()
{
  auto history  = DapDataLocal::instance()->bugReportHistory();

  if (history->isEmpty())
    {
      beginResetModel();
      DapDataLocal::instance()->bugReportHistory()->bagreportListLoaded();
      endResetModel();
    }
}

void DapQmlModelBugReports::_slotNewReport (int a_index)
{
  beginInsertRows (QModelIndex(), a_index, a_index);
  endInsertRows();
}

void DapQmlModelBugReports::_slotRemoved (int a_index)
{
  beginRemoveRows (QModelIndex(), a_index, a_index);
  endRemoveRows();
}

/*-----------------------------------------*/
