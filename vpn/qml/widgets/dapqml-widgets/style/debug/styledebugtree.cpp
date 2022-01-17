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
  , m_root ({0, TreeItem::Root, Line()})
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

bool StyleDebugTree::describe (QString a_name, QStringList a_fields, QObject *a_item)
{
  /* skip when already exists */
  if (m_names.contains (a_name))
    return false;

  m_descriptors << Descriptor (a_name, a_fields, a_item);
  return true;
}

bool StyleDebugTree::undescribe (QString a_name)
{
  /* skip when already exists */
  if (!m_names.contains (a_name))
    return false;

  int index = m_names.indexOf (a_name);
  m_descriptors.removeAt (index);
  m_names.removeAt (index);
  return true;
}

/********************************************
 * OVERRIDE
 *******************************************/

QVariant StyleDebugTree::data (const QModelIndex &index, int role) const
{

}

Qt::ItemFlags StyleDebugTree::flags (const QModelIndex &index) const
{

}

QVariant StyleDebugTree::headerData (int section, Qt::Orientation orientation, int role) const
{

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

  if (parentItem == &m_root)
    return QModelIndex();

  return createIndex (parentItem->row(), 0, parentItem);
}

int StyleDebugTree::rowCount (const QModelIndex &parent) const
{

}

int StyleDebugTree::columnCount (const QModelIndex &parent) const
{

}

/*-----------------------------------------*/

StyleDebugTree::TreeItem::TreeItem()
  : m_row (-1)
  , m_type (None)
{

}

StyleDebugTree::TreeItem::TreeItem (int a_row, Type a_type, Descriptor a_descriptor)
  : m_row (a_row)
  , m_type (a_type)
  , m_item (QVariant::fromValue (a_descriptor))
{

}

StyleDebugTree::TreeItem::TreeItem (int a_row, Type a_type, Line a_line)
  : m_row (a_row)
  , m_type (a_type)
  , m_item (QVariant::fromValue (a_line))
{

}

StyleDebugTree::TreeItem *StyleDebugTree::TreeItem::child (int row) const
{
  /* return none */
  if (m_type != Root || m_type != Item)
    return nullptr; // &(s_item = TreeItem(TreeItem::None, nullptr));

  /* return child of root item */
  if ((m_type == Root) && (row < 0 || row >= s_instance->m_descriptors.size()))
    return nullptr;
  else
    return & (s_item = TreeItem (row, TreeItem::Item, s_instance->m_descriptors[row]));

  /* return child of descriptor item */
  if (m_type == Item)
    {
      Descriptor line = m_item.value<Descriptor>();
      if ((row < 0 || row >= line.fieldsData().size()))
        return nullptr;
      else
        return & (s_item = TreeItem (row, TreeItem::Item, line.fieldsData()[row]));
    }

  /* unknown behavior */
  return nullptr;
}

StyleDebugTree::TreeItem *StyleDebugTree::TreeItem::parentItem() const
{
  if (m_type != Item)
    return nullptr;
  return & (s_item = TreeItem ());
}

int StyleDebugTree::TreeItem::row()
{
  return m_row;
}
