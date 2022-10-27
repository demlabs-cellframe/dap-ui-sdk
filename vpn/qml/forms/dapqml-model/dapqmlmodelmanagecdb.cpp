/* INCLUDES */
#include "dapqmlmodelmanagecdb.h"
#include "dapqml-abstract/abstractcdbmanager.h"

/* VARS */
static QSharedPointer<AbstractCdbManager> s_manager;
static const QString FIELD_SERVER {"server"};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelManageCdb::DapQmlModelManageCdb()
  : QAbstractTableModel()
{

}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelManageCdb *DapQmlModelManageCdb::instance()
{
  static DapQmlModelManageCdb *__inst = nullptr;
  if (__inst == nullptr)
    __inst  = new DapQmlModelManageCdb;
  return __inst;
}

QObject *DapQmlModelManageCdb::singletonProvider (QQmlEngine *, QJSEngine *)
{
  return instance();
}

void DapQmlModelManageCdb::installManager (QSharedPointer<AbstractCdbManager> a_manager)
{
  s_manager = a_manager;
}

int DapQmlModelManageCdb::length() const
{
  return rowCount();
}

inline void parseServerData (const QVariant &a_data, /* out */ AbstractCdbManager::Server &a_server)
{
  auto value  = a_data.toMap();
  if (value.contains (FIELD_SERVER))
    a_server  = AbstractCdbManager::Server (value[FIELD_SERVER].toString());
}

void DapQmlModelManageCdb::add (const QVariant &a_data)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return;

  /* get new server info */
  AbstractCdbManager::Server newServer;
  parseServerData (a_data, newServer);

  /* store result and update */
  s_manager->append (std::move (newServer));
  s_manager->update();
}

void DapQmlModelManageCdb::edit (int a_index, const QVariant &a_data)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return;

  /* check boundaries */
  if (a_index >= s_manager->size())
    return;

  /* get source item */
  auto itemIndex = s_manager->index (a_index);

  /* modify data */
  parseServerData (a_data, *itemIndex);

  /* store result */
  //s_manager->setServer (a_index, std::move (itemIndex));
  s_manager->update();
}

void DapQmlModelManageCdb::remove (int a_index)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return;

  /* check boundaries */
  if (a_index >= s_manager->size())
    return;

  /* perform removing */
  s_manager->removeAt (a_index);
  s_manager->update();
}

void DapQmlModelManageCdb::refreshContent()
{
  beginResetModel();
  endResetModel();
}

QVariant DapQmlModelManageCdb::value (int a_index, const QString &a_name)
{
  return data (index (a_index, 0), roleNames().key (a_name.toUtf8()));
}

void DapQmlModelManageCdb::doImport(const QString &a_filename)
{
  if (s_manager.isNull())
    return;
  s_manager->doImport (a_filename);
}

void DapQmlModelManageCdb::doExport(const QString &a_filename) const
{
  if (s_manager.isNull())
    return;
  s_manager->doExport (a_filename);
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelManageCdb::rowCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  /* check if manager installed */
  if (s_manager.isNull())
    return 0;

  return s_manager->size();
}

int DapQmlModelManageCdb::columnCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlModelManageCdb::data (const QModelIndex &index, int role) const
{
  /* check if manager installed */
  if (s_manager.isNull())
    return QVariant();

  /* check index */
  if (!index.isValid())
    return QVariant();

  /* check boundaries */
  if (index.row() >= s_manager->size())
    return QVariant();

  /* return value */
  const auto itemIndex = s_manager->cindex (index.row());
  switch (role)
    {

    case 0: // name
      return QString (*itemIndex);

    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelManageCdb::roleNames() const
{
  static QHash<int, QByteArray> names;

  if (names.isEmpty())
    {
      names.insert (0, "name");
    }

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelManageCdb::slotSetup()
{
  /* nothing for now */
}

/*-----------------------------------------*/
