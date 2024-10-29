/* INCLUDES */
#include "stylesheettablemodel.h"
#include "style/dapguistylemanager.h"

class DgsmAccess : public DapGuiStyleManager
{
public:
  static QString styleByClassName (const QString &className)
  {
    return DapGuiStyleManager::styleByClassName (className);
  }
  static QString styleByClassList (const QString &classNameList)
  {
    return DapGuiStyleManager::styleByClassList (classNameList);
  }
  static QString styleByClassList (const QStringList &classNameList)
  {
    return DapGuiStyleManager::styleByClassList (classNameList);
  }
  static QStringList classList()
  {
    return DapGuiStyleManager::classList();
  }
  static DapGuiStyleManager &_signal()
  {
    return DapGuiStyleManager::_signal();
  }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

StyleSheetTableModel::StyleSheetTableModel()
{
  /* signals */
  connect (&DgsmAccess::_signal(), &DapGuiStyleManager::forceStyleUpdate,
           this, &StyleSheetTableModel::forcedStyleUpdate,
           Qt::QueuedConnection);
}

/********************************************
 * OVERRIDE
 *******************************************/

int StyleSheetTableModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED (parent)
  return 2;
}

int StyleSheetTableModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED (parent)
  return DgsmAccess::classList().size();
}

QVariant StyleSheetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  switch (section)
    {
    case 0: return "Class name";
    case 1: return "Content";
    }

  return QVariant();
}

QVariant StyleSheetTableModel::data(const QModelIndex &index, int role) const
{
  /* check role */
  if (role != Qt::DisplayRole)
    return QVariant();

  /* check bounds */
  auto list = DgsmAccess::classList();
  if ((index.row() >= list.size()) || (index.row() < 0))
    return QVariant();

  /* return data by column */
  switch (index.column())
    {
    case 0: return list.value(index.row(), QString());
    case 1: return DgsmAccess::styleByClassName (list.value(index.row(), QString()));
    }

  return QVariant();
}

/********************************************
 * SLOTS
 *******************************************/

void StyleSheetTableModel::forcedStyleUpdate()
{
  beginResetModel();
  endResetModel();
}

/*-----------------------------------------*/
