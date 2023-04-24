/* INCLUDES */
#include "DapRoutingExceptionsList.h"
#include <QJsonArray>
#include <QJsonObject>

#include "DapDataLocal.h"

/* FUNCS */

inline QJsonObject itemToJson (const DapRoutingExceptionsList::App &a_src)
{
  return QJsonObject
  {
    {"name", a_src.packageName}
  };
}

inline DapRoutingExceptionsList::App jsonToItem (const QJsonObject &a_src)
{
  return DapRoutingExceptionsList::App
  {
    a_src.value ("name").toString(),
    QString(),
    QString(),
    false
  };
}


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapRoutingExceptionsList::DapRoutingExceptionsList (QObject *parent)
  : QObject{parent}
  , m_packageListSize (-1)
{

}

void DapRoutingExceptionsList::clearList()
{
  m_packageList.clear();
}

/********************************************
 * METHODS
 *******************************************/

int DapRoutingExceptionsList::size()
{
  return m_packageListSize;
}

void DapRoutingExceptionsList::setSize (int a_size)
{
  m_packageListSize = a_size;
}

const QList<DapRoutingExceptionsList::App> &DapRoutingExceptionsList::apps() const
{
  return m_packageList;
}

const QList<DapRoutingExceptionsList::Route> &DapRoutingExceptionsList::routes() const
{
  static QList<DapRoutingExceptionsList::Route> i; // TODO
  return i;
}

void DapRoutingExceptionsList::save()
{
  QJsonArray array;

  for (const auto &item : qAsConst (m_packageList))
    array << itemToJson (item);

  DapDataLocal::instance()->saveToSettings (ROUTING_EXCEPTIONS_LIST, array);

  while (array.size())
    array.takeAt (array.size() - 1);
}

void DapRoutingExceptionsList::load()
{
  if (!m_packageList.isEmpty())
    return emit sigRoutingExceptionsListLoaded();

  QJsonArray array;

  if (DapDataLocal::instance()->loadFromSettings (ROUTING_EXCEPTIONS_LIST, array)
      && !array.isEmpty())
    {
      m_packageList.clear();
      for (const auto &obj : qAsConst (array))
        {
          auto item = jsonToItem (obj.toObject());
          slotAddAppRoutingExceptionsList (std::move (item));
        }
    }

  emit sigRoutingExceptionsListLoaded();

  while (array.size())
    array.takeAt (array.size() - 1);
}

/********************************************
 * SLOTS
 *******************************************/

void DapRoutingExceptionsList::slotAddPackageInfo (const QString &a_packageName, const QString &appName, const QString &a_icon)
{
  m_packageList << App {a_packageName, appName, a_icon, false};

  if (m_packageList.size() >= m_packageListSize)
    emit sigRoutingExceptionsListFilled();
}

void DapRoutingExceptionsList::slotAddAppRoutingExceptionsList (const App &a_item)
{
  m_routingExceptionsPackList << a_item;
}

/*-----------------------------------------*/
