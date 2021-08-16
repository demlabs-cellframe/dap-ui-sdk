#ifndef DAPCMDFACTORY_H
#define DAPCMDFACTORY_H

/* INCLUDES */
#include "DapCmdServiceAbstract.h"
#include <QMap>
#include <QMutex>

/****************************************//**
 * @brief instance container
 * for DapCmdServiceAbstract child classes
 * @note for service use
 *
 * - @ref instance() to get singleton instance
 *
 * - @ref operator[] to instanciate or/and get instance of DapCmd
 *
 * - @ref get(const Type &t) same as operator[], but also dynamicaly cast return
 *
 * @date 16.08.2021
 * @author mikhail.shilenko
 *******************************************/

class DapCmdFactory
{
  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Type
  {
    AndroidTunnel,
    Auth,
    Connect,
    LastConnectionData,
    News,
    PingServer,
    Purchase,
    SendBugReport,
    ServerData,
    ServerList,
    SignUp,
    States,
    Stats,
    TunTap,
    UserData,
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QMap<Type, DapCmdServiceAbstract*> m_cmds; ///< container
  static QMutex m_mutex;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
private:
  /* define */

  explicit DapCmdFactory();
  ~DapCmdFactory();

  /* delete */

  DapCmdFactory(const DapCmdFactory&) = delete;
  DapCmdFactory(DapCmdFactory&&) = delete;
  /// @}

  /****************************************//**
   * @name STATIC METHODS
   *******************************************/
  /// @{
public:
  /// get singleton instance
  static DapCmdFactory *instance();
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  /// overlap operator[] for usability
  template<typename item>
  item *get (const Type &t)
  {
    return dynamic_cast<item *> ((*this)[t]);
  }
  /// @}

  /****************************************//**
   * @name PUBLIC OPERATORS
   *******************************************/
  /// @{
public:
  /* define */

  /// get cmd handler by type
  DapCmdServiceAbstract *operator [] (const Type &t);

  /* delete */

  DapCmdFactory &operator=(const DapCmdFactory&) = delete;
  DapCmdFactory &operator=(DapCmdFactory&&) = delete;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDFACTORY_H
