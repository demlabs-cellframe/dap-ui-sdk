#ifndef STYLEDEBUGTREE_H
#define STYLEDEBUGTREE_H

/* INCLUDES */
#include <QAbstractItemModel>
#include "styledebugitemdescriptor.h"

/****************************************//**
 * @brief style debug tree
 *
 * Allows to dump information from qml items
 * and show it inside treeview
 * @ingroup groupDapGuiStyle
 * @date 17.01.2022
 * @author Mikhail Shilenko
 *******************************************/

class StyleDebugTree : public QAbstractItemModel
{
  Q_OBJECT

  friend class TreeItem;

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  using Descriptor  = StyleDebugItemDescriptor;
  using Line        = StyleDebugItemDescriptor::Line;
  class TreeItem
  {
  public:
    enum Type
    {
      None, Root, Item, Field
    };

  protected:
    int m_row;
    Type m_type;
    QVariant m_item;

  public:
    TreeItem();
    TreeItem (int a_row, Type a_type, Descriptor a_descriptor);
    TreeItem (int a_row, Type a_type, Line a_line);

    TreeItem *child (int row) const;
    TreeItem *parentItem() const;
    int row();
  };

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QList<Descriptor> m_descriptors;
  QStringList m_names;
  TreeItem m_root;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit StyleDebugTree (QObject *parent = nullptr);
  ~StyleDebugTree();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE static StyleDebugTree *instance();

  Q_INVOKABLE bool describe (QString a_name, QStringList a_fields, QObject *a_item);
  Q_INVOKABLE bool undescribe (QString a_name);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  QVariant data (const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags (const QModelIndex &index) const override;
  QVariant headerData (int section, Qt::Orientation orientation,
                       int role = Qt::DisplayRole) const override;
  QModelIndex index (int row, int column,
                     const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent (const QModelIndex &index) const override;
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  int columnCount (const QModelIndex &parent = QModelIndex()) const override;
  /// @}
};

/*-----------------------------------------*/
#endif // STYLEDEBUGTREE_H
