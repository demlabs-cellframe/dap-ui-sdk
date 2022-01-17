/* INCLUDES */
#include "styledebugitemdescriptor.h"
#include <QPair>
#include <QVariant>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

StyleDebugItemDescriptor::StyleDebugItemDescriptor()
  : m_item (nullptr)
{

}

StyleDebugItemDescriptor::StyleDebugItemDescriptor (QString a_name, QStringList a_fields, QObject *a_item)
  : m_name (a_name)
  , m_fields (a_fields)
  , m_item (a_item)
{
  update();
}

StyleDebugItemDescriptor::StyleDebugItemDescriptor (const StyleDebugItemDescriptor &a_src)
  : m_name (a_src.m_name)
  , m_fields (a_src.m_fields)
  , m_item (a_src.m_item)
  , m_fieldData (a_src.m_fieldData)
{

}

StyleDebugItemDescriptor::StyleDebugItemDescriptor (StyleDebugItemDescriptor &&a_src)
  : m_name (std::move (a_src.m_name))
  , m_fields (std::move (a_src.m_fields))
  , m_item (std::move (a_src.m_item))
  , m_fieldData (std::move (a_src.m_fieldData))
{

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

/********************************************
 * OPERATORS
 *******************************************/

StyleDebugItemDescriptor &StyleDebugItemDescriptor::operator=(const StyleDebugItemDescriptor &a_src)
{
  m_name      = a_src.m_name;
  m_fields    = a_src.m_fields;
  m_item      = a_src.m_item;
  m_fieldData = a_src.m_fieldData;
  return *this;
}

StyleDebugItemDescriptor &StyleDebugItemDescriptor::operator=(StyleDebugItemDescriptor &&a_src)
{
  m_name      = std::move (a_src.m_name);
  m_fields    = std::move (a_src.m_fields);
  m_item      = std::move (a_src.m_item);
  m_fieldData = std::move (a_src.m_fieldData);
  return *this;
}

/*-----------------------------------------*/
