#ifndef ABSTRACTSERVERLISTMODELBRIDGE_H
#define ABSTRACTSERVERLISTMODELBRIDGE_H

/* INCLUDES */
#include "dapqml-model/dapqmlmodelautoserverlist.h"

/****************************************//**
 * @brief abstract server list model bridge
 *
 * Provide access to required models
 *
 * Used to install different models
 *
 * @ingroup groupUiModels
 * @date 23.02.2023
 * @author Mikhail Shilenko
 *******************************************/

class AbstractServerListModelBridge
{
  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  AbstractServerListModelBridge();
  virtual ~AbstractServerListModelBridge();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// returns bridge that provides default models
  static AbstractServerListModelBridge *getDefaultBridge();
  /// @}

  /****************************************//**
   * @name VIRTUAL METHODS
   *******************************************/
  /// @{
public:
  /// get proper server list
  virtual DapSortedServerList *serverList() const = 0;
  /// get proper auto server list
  virtual DapQmlModelAutoServerList *autoServerList() const = 0;
  /// handle custom data
  /// @note to setup handling define customRoleNames
  virtual QVariant customData (const QModelIndex &a_index, int a_role, int a_sizeAuto, int a_size) const;
  /// custom role names that can be handle by this bridge
  /// @note this can also replace old roles
  virtual const QHash<int, QByteArray> &customRoleNames() const;
  /// @}

  /****************************************//**
   * @name DISABLE COPY
   *******************************************/
  /// @{
public:
  AbstractServerListModelBridge (const AbstractServerListModelBridge &) = delete;
  AbstractServerListModelBridge &operator= (const AbstractServerListModelBridge &) = delete;

  AbstractServerListModelBridge (AbstractServerListModelBridge &&) = delete;
  AbstractServerListModelBridge &operator= (AbstractServerListModelBridge &&) = delete;
  /// @}
};

/*-----------------------------------------*/
#endif // ABSTRACTSERVERLISTMODELBRIDGE_H
