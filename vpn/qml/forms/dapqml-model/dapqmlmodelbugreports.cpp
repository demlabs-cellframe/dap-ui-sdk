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

  return DapDataLocal::instance()->bugReportHistory()->getBugReportsList()->size();
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
  auto list   = DapDataLocal::instance()->bugReportHistory()->getBugReportsList();
  if (index.row() >= list->size())
    return QVariant();

  /* variables */
  auto column = Column (role);
  auto item   = list->at (index.row());

  /* return value by type */
  switch (column)
    {
    case C_NUMBER:  return "Report #" + item.number;
    case C_STATUS:  return item.status;
    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelBugReports::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "name");
  names.insert (1, "state");

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelBugReports::slotSetup()
{
  beginResetModel();
  DapDataLocal::instance()->bugReportHistory()->loadHistoryBugReportData();
  endResetModel();
}

/*-----------------------------------------*/
