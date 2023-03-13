#ifndef DAPQMLSERVERMANAGER_H
#define DAPQMLSERVERMANAGER_H

/* INCLUDES */
#include <QObject>
#include "DapServerList.h"

/****************************************//**
 * @brief manage servers interface
 * @ingroup groupUiModels
 * @date 13.03.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlServerManager : public QObject
{
  Q_OBJECT

  friend class ManageServersSaveDelay;

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum CustomFieldId
  {
    favorite  = Qt::UserRole + 128,
    deleted,
    custom,
  };
  Q_ENUM(CustomFieldId)
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapQmlServerManager();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlServerManager *instance();

  /// return if manager is enabled
  bool enabled() const;
  /// set manager enabled state
  void setEnabled (bool a_enable);

  /// import new servers from node db
  void importServers (const DapServerInfoList *a_servers);
  /// create new server
  void append (const QVariant &a_data);
  /// replace (edit) old server with new one
  void replace (int a_index, const QVariant &a_data);
  /// remove server by index
  void remove (int a_index);
  /// switch server favorite state
  void fave (int a_index);
  /// get server info
  const DapServerInfo &at (int a_index) const;
  /// server amount
  int size() const;

  /// from config
  void load();
  /// to config
  void save();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSERVERMANAGER_H
