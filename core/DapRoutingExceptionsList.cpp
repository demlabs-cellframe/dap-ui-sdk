#include "DapRoutingExceptionsList.h"
#include <QJsonArray>
#include <QJsonObject>

#include "DapDataLocal.h"

inline QJsonObject itemToJson (const DapRoutingExceptionsListApp &a_src)
{
  return QJsonObject
  {
    {"name", a_src.m_packageName}
  };
}

inline DapRoutingExceptionsListApp jsonToItem (const QJsonObject &a_src)
{
  return DapRoutingExceptionsListApp
  {
    a_src.value ("name").toString(),
    QString(),
    QString()
  };
}


DapRoutingExceptionsList::DapRoutingExceptionsList(QObject *parent)
    : QObject{parent}
{

}

void DapRoutingExceptionsList::clearList()
{
    m_packageList.clear();
}

void DapRoutingExceptionsList::addPackageInfo(QString packageName, QString appName, QString icon){
    m_packageList.insert(m_packageList.size(), DapRoutingExceptionsListApp(packageName, appName, icon));
    if (m_packageList.size() >= packageListSize)
        emit sigRoutingExceptionsListFilled();
}

void DapRoutingExceptionsList::addAppRoutingExceptionsList(DapRoutingExceptionsListApp item)
{
  m_routingExceptionsPackList.insert(m_routingExceptionsPackList.size(), item);
}

QList<DapRoutingExceptionsListApp> DapRoutingExceptionsList::apps() const
{
  return m_packageList.values();
}

QList<DapRoutingExceptionsListRoute> DapRoutingExceptionsList::routes() const
{
  return QList<DapRoutingExceptionsListRoute>(); // TODO
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
            addAppRoutingExceptionsList(std::move (item));
        }
    }

  emit sigRoutingExceptionsListLoaded();

  while (array.size())
    array.takeAt (array.size() - 1);
}
