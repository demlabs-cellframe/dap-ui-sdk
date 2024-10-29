/* INCLUDES */
#include "styledebugitemdescriptor.h"
#include "styledebugtree.h"
#include <QPair>
#include <QVariant>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

StyleDebugItemDescriptor::StyleDebugItemDescriptor()
  : QObject()
  , m_item (nullptr)
{

}

StyleDebugItemDescriptor::StyleDebugItemDescriptor (QString a_name, QStringList a_fields, QObject *a_item)
  : QObject()
  , m_name (a_name)
  , m_fields (a_fields)
  , m_item (a_item)
{
  update();
  _reconnectItem();
}

StyleDebugItemDescriptor::StyleDebugItemDescriptor (const StyleDebugItemDescriptor &a_src)
  : QObject (a_src.parent())
  , m_name (a_src.m_name)
  , m_fields (a_src.m_fields)
  , m_item (a_src.m_item)
  , m_fieldData (a_src.m_fieldData)
{
  _reconnectItem();
}

StyleDebugItemDescriptor::StyleDebugItemDescriptor (StyleDebugItemDescriptor &&a_src)
  : QObject (a_src.parent())
  , m_name (std::move (a_src.m_name))
  , m_fields (std::move (a_src.m_fields))
  , m_item (std::move (a_src.m_item))
  , m_fieldData (std::move (a_src.m_fieldData))
{
  _reconnectItem();
}

StyleDebugItemDescriptor::~StyleDebugItemDescriptor()
{

}

/********************************************
 * METHODS
 *******************************************/

const QString &StyleDebugItemDescriptor::name() const
{
  return m_name;
}

void StyleDebugItemDescriptor::setName(const QString &newName)
{
  m_name = newName;
}

const QStringList &StyleDebugItemDescriptor::fields() const
{
  return m_fields;
}

void StyleDebugItemDescriptor::setFields(const QStringList &newFields)
{
  m_fields = newFields;
}

QObject *StyleDebugItemDescriptor::item() const
{
  return m_item;
}

void StyleDebugItemDescriptor::setItem(QObject *newItem)
{
  m_item = newItem;
}

const QList<QPair<QString, QString>> &StyleDebugItemDescriptor::fieldsData() const
{
  return m_fieldData;
}

void StyleDebugItemDescriptor::update()
{
  /* check */
  if (m_item == nullptr)
    return;

  /* clear storage */
  m_fieldData.clear();

  /* add first field */
  m_fieldData << Line{"name", name()};

  /* get and store all fields */
  for (auto i = m_fields.cbegin(), e = m_fields.cend(); i != e; i++)
    {
      auto value  = m_item->property(i->toUtf8());
      Line info;
      info.first  = *i;
      info.second = QString ("%1:%2").arg (value.typeName(), value.toString());
      m_fieldData << info;
    }
}

void StyleDebugItemDescriptor::_reconnectItem()
{
  if (m_conn)
    QObject::disconnect (m_conn);

  if (m_item)
    m_conn  = connect (m_item, &QObject::destroyed,
                       this, &StyleDebugItemDescriptor::slotOnDestroyed);
}

/********************************************
 * SLOTS
 *******************************************/

void StyleDebugItemDescriptor::slotOnDestroyed(QObject *a_ptr)
{
  if (a_ptr != m_item)
    return;
  m_item = nullptr;
  StyleDebugTree::instance()->undescribe (m_name); // m_name  = m_name + " #destroyed";
}

/********************************************
 * OPERATORS
 *******************************************/

StyleDebugItemDescriptor &StyleDebugItemDescriptor::operator=(const StyleDebugItemDescriptor &a_src)
{
  m_name      = a_src.m_name;
  m_fields    = a_src.m_fields;
  m_item      = a_src.m_item;
  m_fieldData = a_src.m_fieldData;

  /* reconnect */
  _reconnectItem();

  return *this;
}

StyleDebugItemDescriptor &StyleDebugItemDescriptor::operator=(StyleDebugItemDescriptor &&a_src)
{
  m_name      = std::move (a_src.m_name);
  m_fields    = std::move (a_src.m_fields);
  m_item      = std::move (a_src.m_item);
  m_fieldData = std::move (a_src.m_fieldData);

  /* reconnect */
  _reconnectItem();

  return *this;
}

/*-----------------------------------------*/
