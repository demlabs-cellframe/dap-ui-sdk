/* INCLUDES */
#include "dapqmlmodelmanagecdb.h"
#include "dapqml-abstract/abstractcdbmanager.h"
#include <QDebug>
#include <QRegExpValidator>

/* VARS */
static QSharedPointer<AbstractCdbManager> s_manager;
static const QString FIELD_SERVER {"server"};
static const QString FIELD_PORT   {"port"};

/* DEFS */
class DapQmlModelManageCdbRowsCtl
{
  struct Row
  {
    QObject *object;
    QMetaObject::Connection connections[2];
  };

  QMap<QObject *, Row> rows;
  DapQmlModelManageCdb *model;
public:
  DapQmlModelManageCdbRowsCtl (DapQmlModelManageCdb *a_model)
    : model (a_model) {}

  void append (QObject *a_row)
  {
    /* check if already here */
    if (rows.contains (a_row))
      return;

    /* connect to update */
    auto conn1  = QObject::connect (model, &DapQmlModelManageCdb::sigMoveFilterChanged,
                                    a_row, [a_row, this]()
      {
        if (a_row->property ("isDragged").toBool())
          return;

        auto index  = a_row->property ("myIndex").toInt();
        auto label  = model->value (index, "name");
        a_row->setProperty ("mainText", label);

        //qDebug() << QString ("changed row:%1:%2").arg (index).arg (label.toString());
      });

    /* connect to destroy */
    auto conn2  = QObject::connect (a_row, &QObject::destroyed,
                                    a_row, [a_row, this]()
      {
        remove (a_row);
      });

    /* store */
    rows.insert (a_row,
                 Row {a_row, {conn1, conn2}});
  }

  void remove (QObject *a_row)
  {
    auto &it  = rows[a_row];
    QObject::disconnect (it.connections[0]);
    QObject::disconnect (it.connections[1]);
    rows.remove (a_row);
  }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelManageCdb::DapQmlModelManageCdb()
  : QAbstractTableModel()
  , d (new DapQmlModelManageCdbRowsCtl (this))
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

static bool parseServerData (DapQmlModelManageCdb *a_model, const QVariant &a_data, /* out */ AbstractCdbManager::CdbServer &a_server)
{
  AbstractCdbManager::CdbServer item;

  /* get value */
  auto value  = a_data.toMap();

  /* parse */
  if (value.contains (FIELD_SERVER))
    item = AbstractCdbManager::CdbServer{
        value[FIELD_SERVER].toString(),
        value[FIELD_PORT].toInt()
      };

  /* validate */
  bool isValid  = false;
  {
    /* reg exps & validators */
    static QRegExp rx [2] = {
      QRegExp {"(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])"},
      QRegExp {"^(0|[1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$"},
    };

    static QRegExpValidator v1{rx[0]};
    static QRegExpValidator v2{rx[1]};

    /* values */
    int pos       = 0;
    auto portName = value[FIELD_PORT].toString();

    /* validation results */
    QValidator::State result[2] = {
      v1.validate (item.address, pos),
      v2.validate (portName, pos),
    };

    /* final result */
    isValid = QValidator::Acceptable == result[0]
        && QValidator::Acceptable == result[1];

    /* error notify */
    if (QValidator::Acceptable != result[0]
        && QValidator::Acceptable != result[1])
      emit a_model->sigError ("invalid address and port");

    else if (QValidator::Acceptable != result[0])
      emit a_model->sigError ("invalid address");

    else if (QValidator::Acceptable != result[1])
      emit a_model->sigError ("invalid port");

  }

  /* store result */
  if (isValid)
    a_server    = item;

  return isValid;
}

bool DapQmlModelManageCdb::add (const QVariant &a_data)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return false;

  /* get new server info */
  AbstractCdbManager::CdbServer newServer;
  if (!parseServerData (this, a_data, newServer))
    return false;

  /* store result and update */
  s_manager->append (std::move (newServer));
  s_manager->update();

  return true;
}

bool DapQmlModelManageCdb::edit (int a_index, const QVariant &a_data)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return false;

  /* check boundaries */
  if (a_index >= s_manager->size())
    return false;

  /* get source item */
  auto itemIndex = s_manager->index (a_index);

  /* modify data */
  if (!parseServerData (this, a_data, *itemIndex))
    return false;

  /* store result */
  //s_manager->setServer (a_index, std::move (itemIndex));
  s_manager->update();

  return true;
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

void DapQmlModelManageCdb::move(int a_firstIndex, int a_secondIndex)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return;

  /* perform moving */
  s_manager->move (a_firstIndex, a_secondIndex);
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

void DapQmlModelManageCdb::setMoveFilter (int a_from, int a_to)
{
  if (s_manager.isNull())
    return;
  emit sigSetMoveFilter (a_from, a_to);
}

void DapQmlModelManageCdb::regRow(QObject *a_row)
{
  d->append (a_row);
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
      return itemIndex->address;
    case 1: // port
      return itemIndex->port;

    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelManageCdb::roleNames() const
{
  static QHash<int, QByteArray> names;

  if (names.isEmpty())
    {
      names.insert (0, "name");
      names.insert (1, "port");
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
