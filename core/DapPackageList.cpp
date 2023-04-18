#include "DapPackageList.h"
#include <QJsonArray>
#include <QJsonObject>

/* LINKS */
inline QJsonObject itemToJson (const DapPackageListItem &a_src)
{
  return QJsonObject
  {
    {"name", a_src.mPackageName}
  };
}

inline DapPackageListItem jsonToItem (const QJsonObject &a_src)
{
  return DapPackageListItem
  {
    a_src.value ("name").toString(),
    QString(),
    QImage()
  };
}


DapPackageList::DapPackageList(QObject *parent)
    : QObject{parent}
{

}

void DapPackageList::clearList()
{
    m_packListItems.clear();
}

void DapPackageList::addItem(QString name, QString lable, QImage icon)
{
    if (m_packListItems.isEmpty())
        m_packListItems.insert(m_packListItems.size(), DapPackageListItem(name, lable, icon));
    m_packListItems.insert(m_packListItems.size() + 1, DapPackageListItem(name, lable, icon));
}

void DapPackageList::addAppToExcludeForRoutingList(DapPackageListItem item)
{
    if (m_packListItems.isEmpty())
        m_exceptionRoutingPackListItem.insert(m_packListItems.size(), item);
    m_exceptionRoutingPackListItem.insert(m_packListItems.size()+1, item);
}


void DapPackageList::save()
{
  /* vars */
  QJsonArray array;

  /* fill in cycle */
  for (const auto &item : qAsConst (m_exceptionRoutingPackListItem))
    array << itemToJson (item);

  /* store into settings */
  DapDataLocal::instance()->saveToSettings (EXCLUDE_FROM_ROUTING_LIST, array);

  /* clear memory */
  while (array.size())
    array.takeAt (array.size() - 1);
}

void DapPackageList::load()
{
  /* do not load list again */
  if (!m_exceptionRoutingPackListItem.isEmpty())
    return emit sigExceptionFromRoutingListLoaded();

  /* vars */
  QJsonArray array;

  /* get array from settings */
  if (DapDataLocal::instance()->loadFromSettings (EXCLUDE_FROM_ROUTING_LIST, array)
      && !array.isEmpty())
    {
      /* parse array */
      m_exceptionRoutingPackListItem.clear();
      for (const auto &obj : qAsConst (array))
        {
          auto item = jsonToItem (obj.toObject());
            addAppToExcludeForRoutingList(std::move (item));
        }
    }

  /* notify */
  emit sigExceptionFromRoutingListLoaded();

  /* clear memory */
  while (array.size())
    array.takeAt (array.size() - 1);
}

