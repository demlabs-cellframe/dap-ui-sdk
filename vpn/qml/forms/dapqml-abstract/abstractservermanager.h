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
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// server type
  enum class ServerType
  {
    INVALID,    ///< dummy invalid
    AUTO,       ///< auto suggestion
    NODE,       ///< provided by node
    NODEHIDDEN, ///< provided by node, but deleted by user
    USER        ///< created by user
  };

  /// Server info struct
  struct Server
  {
    QString name;
    QString location;
    QString address;
    int port;
    ServerType type;
    bool favorite;
  };

  /// Server state struct
  struct State
  {
    int ping;
    int connectionQuality;
  };

  /// combined structure to ease iteration
  struct ServerInfo : public Server, public State
  {
    ServerInfo (const Server &a_server, const State &a_state)
    {
      name              = a_server.name;
      location          = a_server.location;
      address           = a_server.address;
      port              = a_server.port;
      type              = a_server.type;
      favorite          = a_server.favorite;
      ping              = a_state.ping;
      connectionQuality  = a_state.connectionQuality;
    }
  };
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  AbstractServerManager() {};
  virtual ~AbstractServerManager() {};
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// append server
//  void append (Server &&a_newServer)              { setServer (std::move (a_newServer)); }
//  void append (const DapServerInfo &a_newServer)  { append (Server {a_newServer.name, a_newServer.address, a_newServer.port, false}); }
  ServerInfo info (int a_index) const             { return ServerInfo { server (a_index), state (a_index) }; }
//  ServerInfo info (const QString &a_ip) const     { return ServerInfo { server (a_ip), state (a_ip) }; }
  /// @}

  /****************************************//**
   * @name VIRTUAL METHODS
   *******************************************/
  /// @{
public:
  /// request const instance of server
  virtual const Server &server (int a_index) const = 0;

  /// request const instance of server
  virtual const Server &server (const QString &a_ip, bool *a_ok = nullptr) const = 0;

  /// replace server
  virtual void setServer (int a_index, Server &&a_newServer) = 0;

  /// replace server
  virtual void setServer (Server &&a_newServer) = 0;

  /// append new server
  virtual void append (Server &&a_newServer) = 0;

  /// remove server
  virtual void remove (int a_index) = 0;

  /// get index of a_ip. otherwise returns -1
  virtual int indexOf (const QString &a_ip) const = 0;

  /// request server state
  virtual const State &state (int a_index) const = 0;

  /// request server state. otherwise will return static dummy item.
  virtual const State &state (const QString &a_ip, bool *a_ok = nullptr) const = 0;

  /// set specific server state
  virtual void setState (const QString &a_ip, State &&a_newState) = 0;

  /// request servers list size
  virtual int size() const = 0;

  /// request all favorite servers
  virtual const QList<QString> &favorites() const = 0;

  /// sync & update manager data
  virtual void update() const = 0;

  virtual void importFromFile (const QString &a_filename) = 0;
  virtual void exportToFile (const QString &a_filename) const = 0;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  AbstractServerManager &operator<< (Server &&a_newServer)              { append (std::move (a_newServer)); return *this; }
//  AbstractServerManager &operator<< (const DapServerInfo &a_newServer)  { append (a_newServer); return *this; }
  const ServerInfo operator[] (int a_index) const
  {
    return info (a_index);
  }
  /// @}
};

/*-----------------------------------------*/
#endif // ABSTRACTSERVERMANAGER_H
