/* INCLUDES */
#include "styledebugtree.h"

/* VARS */
static StyleDebugTree *s_instance = nullptr;
static thread_local StyleDebugTree::TreeItem s_item;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

StyleDebugTree::StyleDebugTree (QObject *parent)
  : QAbstractItemModel (parent)
{
  s_instance  = this;
}

StyleDebugTree::~StyleDebugTree()
{

}

/********************************************
 * METHODS
 *******************************************/

StyleDebugTree *StyleDebugTree::instance()
{
  if (s_instance == nullptr)
    s_instance  = new StyleDebugTree;
  return s_instance;
}

QObject *StyleDebugTree::singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED (engine)
  Q_UNUSED (scriptEngine)
  return StyleDebugTree::instance();
}

bool StyleDebugTree::describe (QString a_name, QStringList a_fields, QObject *a_item)
{
  /* skip when already exists */
  if (m_names.contains (a_name))
    return false;

  m_root.append (new TreeItem (Descriptor (a_name, a_fields, a_item), &m_root));
  m_names.append (a_name);
  return true;
}

bool StyleDebugTree::undescribe (QString a_name)
{
  /* skip when already exists */
  if (!m_names.contains (a_name))
    return false;

  int index = m_names.indexOf (a_name);
  m_root.remove (index); // m_descriptors.removeAt (index);
  m_names.removeAt (index);
  return true;
}

void StyleDebugTree::update()
{
  emit layoutAboutToBeChanged(); // beginResetModel();
  for (int i = 0, e = m_root.childCount(); i < e; i++)
    {
      auto child  = m_root.child (i);
      child->update();
    }
  emit layoutChanged(); // endResetModel();
}

void StyleDebugTree::redraw()
{
  beginResetModel();
  endResetModel();
  emit emptyChanged();
}

bool StyleDebugTree::isEmpty()
{
  return rowCount() == 0;
}

/********************************************
 * OVERRIDE
 *******************************************/

QVariant StyleDebugTree::data (const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

//  if (role != Qt::DisplayRole)
//    return QVariant();

  TreeItem *item = static_cast<TreeItem *> (index.internalPointer());

  return item->data (role);//(index.column());
}

Qt::ItemFlags StyleDebugTree::flags (const QModelIndex &index) const
{
  return QAbstractItemModel::flags (index);
}

QVariant StyleDebugTree::headerData (int section, Qt::Orientation orientation, int role) const
{
  Q_UNUSED (section)
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  switch (role)
    {
    case 0: return "Name";
    case 1: return "Value";
    }

  return QVariant();
}

QModelIndex StyleDebugTree::index (int row, int column, const QModelIndex &parent) const
{
  /* check if valid */
  if (!hasIndex (row, column, parent))
    return QModelIndex();

  /* vars */
  const TreeItem *parentItem;

  /* get pointer */
  if (!parent.isValid())
    parentItem = &m_root;
  else
    parentItem = static_cast<TreeItem *> (parent.internalPointer());

  /* get child item */
  TreeItem *childItem = parentItem->child (row);

  /* return valid index */
  if (childItem)
    return createIndex (row, column, childItem);

  /* return invalid index */
  return QModelIndex();
}

QModelIndex StyleDebugTree::parent (const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem *> (index.internalPointer());
  TreeItem *parentItem = childItem->parentItem();

  if (parentItem == nullptr || parentItem == &m_root)
    return QModelIndex();

  return createIndex (parentItem->row(), 0, parentItem);
}

int StyleDebugTree::rowCount (const QModelIndex &parent) const
{
  const TreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = &m_root;
  else
    parentItem = static_cast<TreeItem *> (parent.internalPointer());

  return parentItem->childCount();
}

int StyleDebugTree::columnCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent)
  return 2;
}

QHash<int, QByteArray> StyleDebugTree::roleNames() const
{
  QHash<int, QByteArray> names;

  names[0]  = "name";
  names[1]  = "value";

  return names;
}

/*-----------------------------------------*/



StyleDebugTree::TreeItem::TreeItem (TreeItem *a_parentItem)
  : m_parent (a_parentItem)
{

}

StyleDebugTree::TreeItem::TreeItem (Descriptor a_descriptor, TreeItem *a_parentItem)
  : m_parent (a_parentItem)
  , m_item (QVariant::fromValue (a_descriptor))
{
  /* append fields as children */
  for (auto i = a_descriptor.fieldsData().cbegin(), e = a_descriptor.fieldsData().cend(); i != e; i++)
    append (new TreeItem (*i, this));
}

StyleDebugTree::TreeItem::TreeItem (Line a_line, TreeItem *a_parentItem)
  : m_parent (a_parentItem)
  , m_item (QVariant::fromValue (a_line))
{

}

int StyleDebugTree::TreeItem::append(TreeItem *a_child)
{
  int row = m_childItems.size();
  m_childItems.append (a_child);
  return row;
}

void StyleDebugTree::TreeItem::remove (int a_index)
{
  m_childItems.removeAt (a_index);
}

void StyleDebugTree::TreeItem::remove (QString a_name)
{
  for (qsizetype i = 0, e = m_childItems.size(); i != e; i++)
    {
      auto item = m_childItems[i]->m_item;

      if (QString (item.typeName()) == "StyleDebugItemDescriptor")
        {
          if (item.value<Descriptor>().name() == a_name)
            {
              m_childItems.removeAt(i);
              return;
            }
        }
    }
}

StyleDebugTree::TreeItem *StyleDebugTree::TreeItem::child (int row) const
{
  if (row < 0 || row >= m_childItems.size())
    return nullptr;
  return m_childItems[row];
}

StyleDebugTree::TreeItem *StyleDebugTree::TreeItem::parentItem() const
{
  return m_parent;
}

QVariant StyleDebugTree::TreeItem::data (int column) const
{
  if (!m_item.isValid())
    return QVariant();

  auto type = QString (m_item.typeName());

  if (type == "StyleDebugItemDescriptor")
    {
      switch(column)
        {
        case 0: return m_item.value<Descriptor>().name();
        case 1: return QVariant();
        }
    }

  if (type == "StyleDebugItemDescriptor::Line")
    {
      Line line = m_item.value<Line> ();
      switch(column)
        {
        case 0: return line.first;
        case 1: return line.second;
        }
    }

  return QVariant();
}

int StyleDebugTree::TreeItem::row() const
{
  if (m_parent == nullptr)
    return 0;

  for (qsizetype i = 0, e = m_parent->m_childItems.size(); i < e; i++)
    if (m_parent->m_childItems.at(i) == this)
      return i;

  return 0;
}

int StyleDebugTree::TreeItem::childCount() const
{
  return m_childItems.size();
}

void StyleDebugTree::TreeItem::update()
{
  /* protect type */
  auto type = QString (m_item.typeName());
  if (type != "StyleDebugItemDescriptor")
    return;

  /* update fields data */
  auto desc = m_item.value<Descriptor> ();
  desc.update();
  m_item    = QVariant::fromValue (desc);

  /* skip, if not same children amount*/
  if (m_childItems.size() != desc.fieldsData().size())
    return;

  /* update fields tree representations */
  for (qsizetype i = 0, e = m_childItems.size(); i != e; i++)
    {
      auto child  = m_childItems.at(i);
      auto data   = desc.fieldsData().value(i);
      child->update (data);
    }
}

void StyleDebugTree::TreeItem::update(Line a_line)
{
  /* protect type */
  auto type = QString (m_item.typeName());
  if (type != "StyleDebugItemDescriptor::Line")
    return;

  m_item  = QVariant::fromValue (a_line);
}

/*-----------------------------------------*/
