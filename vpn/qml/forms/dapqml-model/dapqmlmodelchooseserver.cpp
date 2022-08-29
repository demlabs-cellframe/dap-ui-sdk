/* INCLUDES */
#include "dapqmlmodelchooseserver.h"
#include "DapServersData.h"

/* VARS */
static DapQmlModelChooseServer *__inst = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelChooseServer::DapQmlModelChooseServer (QObject *parent)
  : QAbstractListModel (parent)
{
  /* vars */
  __inst  = this;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelChooseServer *DapQmlModelChooseServer::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelChooseServer;
  return __inst;
}

void DapQmlModelChooseServer::refresh()
{
  beginResetModel();
  endResetModel();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelChooseServer::rowCount(const QModelIndex &parent) const
{
  return DapServersData::instance()->rowCount (parent);
}

QVariant DapQmlModelChooseServer::data(const QModelIndex &index, int role) const
{
  if (role != 0)
    return QVariant();

  return DapServersData::instance()->data (index, Qt::DisplayRole);
}

QHash<int, QByteArray> DapQmlModelChooseServer::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "name");

  return names;
}

/*-----------------------------------------*/
