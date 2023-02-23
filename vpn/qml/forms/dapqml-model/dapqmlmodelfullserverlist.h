#ifndef DAPQMLMODELFULLSERVERLIST_H
#define DAPQMLMODELFULLSERVERLIST_H

/* INCLUDES */
#include <QAbstractTableModel>

/* DEFS */
class AbstractServerListModelBridge;

/****************************************//**
 * @brief full server list model
 *
 * Model provides list of servers combined
 * with auto servers
 *
 * @ingroup groupUiModels
 * @date 23.02.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelFullServerList : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  AbstractServerListModelBridge *m_bridge;
  QHash<int, QByteArray> _roleNamesMap;
  struct
  {
    int autoServer;
    int server;
    int full;
  } _size;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlModelFullServerList();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  const AbstractServerListModelBridge *bridge() const;
  void setBridge (AbstractServerListModelBridge *a_newBridge);
  Q_INVOKABLE QVariant value (int a_row, const QString &a_name);
protected:
  void _getSizes();
  void _getRoles();
  static const QHash<int, QByteArray> &_baseRoleNames();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELFULLSERVERLIST_H
