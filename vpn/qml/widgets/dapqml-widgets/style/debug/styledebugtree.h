#ifndef STYLEDEBUGTREE_H
#define STYLEDEBUGTREE_H

/* INCLUDES */
#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QJSEngine>
#include "styledebugitemdescriptor.h"

/****************************************//**
 * @brief style debug tree
 *
 * Allows to dump information from qml items
 * and show it inside treeview
 *
 * Used only for debug purposes
 *
 * How to use:
 * - Enable define ENABLE_FORM_DEBUG inside *.pro
 * - Uncomment TreeView inside PageCtlDebug.qml
 * - Describe your qml item (example: Component.onCompleted: StyleDebugTree.describe ("info label", ["x", "y", "width", "height"], this);)
 * - When app is started debug form will appear
 * - When qml items exist (or created), press "Redraw". It will refresh model content
 * - When qml items state changed, press "Update" to refresh values inside debug tree
 *
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
    TreeItem *m_parent;
    QVariant m_item;
    QList<TreeItem*> m_childItems;

  public:
    explicit TreeItem (TreeItem *a_parentItem = nullptr);
    explicit TreeItem (Descriptor a_descriptor, TreeItem *a_parentItem = nullptr);
    explicit TreeItem (Line a_line, TreeItem *a_parentItem = nullptr);

    int append (TreeItem *a_child);
    void remove (int a_index);
    void remove (QString a_name);
    TreeItem *child (int row) const;
    TreeItem *parentItem() const;
    QVariant data(int column) const;
    int row() const;
    int childCount() const;

    /// update field data
    void update();
    void update(Line a_line);
  };

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
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
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine);

  Q_INVOKABLE bool describe (QString a_name, QStringList a_fields, QObject *a_item);
  Q_INVOKABLE bool undescribe (QString a_name);

  /// update data inside descriptors
  Q_INVOKABLE void update();
  Q_INVOKABLE void redraw();
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
  QHash<int, QByteArray> roleNames() const override;
  /// @}
};

/*-----------------------------------------*/
#endif // STYLEDEBUGTREE_H
