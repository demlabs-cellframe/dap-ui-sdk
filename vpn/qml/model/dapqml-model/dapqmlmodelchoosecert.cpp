/* INCLUDES */
#include "dapqmlmodelchoosecert.h"

/* VARS */
static QStringList *s_dummyList       = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelChooseCert::DapQmlModelChooseCert(QObject *parent)
  : QAbstractTableModel{parent}
{
  s_dummyList = new QStringList;

  for (int i = 0; i < 16; i++)
    s_dummyList->append (QString ("Certificate %1").arg(i+1));
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelChooseCert::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return s_dummyList->size();
}

int DapQmlModelChooseCert::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlModelChooseCert::data(const QModelIndex &index, int role) const
{
  /* check index */
  if (!index.isValid())
    return QVariant();

  /* check boundaries */
  if (index.row() >= s_dummyList->size())
    return QVariant();

  if (role == 0)
    return s_dummyList->at(index.row());

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelChooseCert::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "name");

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelChooseCert::slotSetup()
{
  beginResetModel();
  endResetModel();
}

/*-----------------------------------------*/
