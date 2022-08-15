#ifndef ABSTRACTSERVERMANAGER_H
#define ABSTRACTSERVERMANAGER_H

/* INCLUDES */
#include <QtGlobal>
#include <QString>
#include <QStringList>

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
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// Server info struct
  struct Server
  {
    QString name;
    QString address;
    int port;
  };

  /// Server state struct
  struct State
  {
    int ping;
    bool favorite;
  };
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  AbstractServerManager() {};
  virtual ~AbstractServerManager() = 0;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// append server
  void append (Server &&a_newServer)  { setServer (std::move (a_newServer)); }
  QStringList servers() const         { return keys(); }
  /// @}

  /****************************************//**
   * @name VIRTUAL METHODS
   *******************************************/
  /// @{
public:
  /// request const instance of server. otherwise will return static dummy item.
  virtual const Server &server (const QString &a_name) const = 0;

  /// set or replace server
  virtual void setServer (const QString &a_name, Server &&a_newServer) = 0;

  /// same as append server
  /// @note will replace server with the same name
  virtual void setServer (Server &&a_newServer) = 0;

  /// remove server by name
  virtual void remove (const QString &a_name) = 0;

  /// set specific server state
  virtual void setState (const QString &a_name, State &&a_newState) = 0;

  /// request server state. otherwise will return static dummy item.
  virtual const State &state (const QString &a_name) const = 0;

  /// request servers keys
  virtual QStringList keys() const = 0;

  /// request servers list size
  virtual int size() const = 0;

  /// request all favorite servers
  virtual QStringList favorites() const = 0;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  AbstractServerManager &operator<< (Server &&a_newServer)  { append (std::move (a_newServer)); return *this; }
  const Server operator[] (const QString &a_name) const     { return server (a_name); }
  /// @}
};

/*-----------------------------------------*/
#endif // ABSTRACTSERVERMANAGER_H
