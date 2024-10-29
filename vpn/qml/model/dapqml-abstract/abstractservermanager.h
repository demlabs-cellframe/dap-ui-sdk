#ifndef ABSTRACTSERVERMANAGER_H
#define ABSTRACTSERVERMANAGER_H

/* INCLUDES */
#include <QtGlobal>
#include <QString>
#include <QStringList>
#include "DapServerInfo.h"

/****************************************//**
 * @brief abstract server manager
 *
 * Allows to use template method to display
 * and manage servers
 *
 * @ingroup groupUiModels
 * @date 15.08.2022
 * @author Mikhail Shilenko
 *******************************************/

class AbstractServerManager
{
  Q_DISABLE_COPY_MOVE(AbstractServerManager)

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  AbstractServerManager() {};
  virtual ~AbstractServerManager() {};
  /// @}

  /****************************************//**
   * @name VIRTUAL METHODS
   *******************************************/
  /// @{
public:
  /// request const instance of server
  virtual const DapServerInfo &server (int a_index) const = 0;

  /// request const instance of server
  virtual const DapServerInfo &server (const QString &a_ip, bool *a_ok = nullptr) const = 0;

  /// append new server
  virtual void append (DapServerInfo &&a_newServer) = 0;

  /// remove server
  virtual void remove (int a_index) = 0;

  /// get index of a_ip. otherwise returns -1
  virtual int indexOf (const QString &a_ip) const = 0;

  /// request server state
  virtual bool favorite (int a_index) const = 0;

  /// set specific server state
  virtual void setFavorite (const QString &a_ip, bool a_favorite) = 0;

  /// request servers list size
  virtual int size() const = 0;

  /// sync & update manager data
  virtual void update() const = 0;

  virtual void load (const QString &a_filename) = 0;
  virtual void save (const QString &a_filename) const = 0;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  AbstractServerManager &operator<< (DapServerInfo &&a_newServer)       { append (std::move (a_newServer)); return *this; }
  AbstractServerManager &operator<< (const DapServerInfo &a_newServer)  { append (DapServerInfo (a_newServer)); return *this; }
  const DapServerInfo &operator[] (int a_index) const
  {
    return server (a_index);
  }
  /// @}
};

/*-----------------------------------------*/
#endif // ABSTRACTSERVERMANAGER_H
