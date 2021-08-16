#ifndef DAPCMDFACTORY_H
#define DAPCMDFACTORY_H

/* INCLUDES */
#include "DapCmdServiceAbstract.h"
#include <QMap>
#include <QMutex>
#include <QScopedPointer>
#include <typeinfo>

/****************************************//**
 * @brief template instance container factory
 * for DapCmdServiceAbstract child classes.
 * @note for service use.
 * @note class Item is thread-safe.
 *
 * To use this factory: create template Item
 * instance providing the name of class and
 * use it's operators to get instance pointer.
 *
 * Instance of provided class will be created
 * once (singleton), will exist safely inside
 * current thread and can be accessed safely
 * via instantiating template class Item with
 * provided class name, which acts like
 * QMutexLock, but with dereferencind
 * operators ( operator->() and operator&() )
 *
 * @example DapCmdFactory::Item<DapCmdStates>()->sendCmdStates();
 * @example using Factory = DapCmdFactory::Item<T>; Factory<DapCmdStates>()->sendCmdStates();
 * @example {DapCmdFactory::Item<DapCmdStates> op; op->sendCmdStates();}
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
  template<class T>
  class Item
  {
    QMutexLocker *locker;
    T *ptr;
  public:
    Item()
    {
      auto &i = DapCmdFactory::get<T>();
      locker  = new QMutexLocker(i.getLock());
      ptr     = dynamic_cast<T*> (i.getValue());
    }
    ~Item()
    {
      delete locker;
    }
    T *operator->() {return ptr;}
    T *operator &() {return ptr;}
  };
  /// @}

  /****************************************//**
   * @name PROTECTED DEFS
   *******************************************/
  /// @{
protected:
  class Node
  {
    /* VARS */
    QMutex *lock;
    DapCmdServiceAbstract *value;

    /* CONSTRUCT/DESTRUCT */
  public:
    Node (DapCmdServiceAbstract *s)
      : lock (new QMutex)
      , value (s)
    {

    }
    Node () = delete;
    Node (const Node &) = delete;
    Node (Node &&r)
    {
      if(&r == this)
        return;
      lock    = r.lock;
      value   = r.value;
      r.lock  = nullptr;
      r.value = nullptr;
    }
    ~Node()
    {
      auto tl = lock;
      auto tv = value;
      lock    = nullptr;
      value   = nullptr;
      delete tl;
      delete tv;
    }

    /* METHODS */
    template<class T>
    static Node *create()
    {
      return new Node(new T);
    }

    /* OPERATORS */
    Node &operator=(const Node&) = delete;
    Node &operator=(Node &&r)
    {
      lock    = r.lock;
      value   = r.value;
      r.lock  = nullptr;
      r.value = nullptr;
      return *this;
    }
    DapCmdServiceAbstract *getValue() const {return value;};
    QMutex *getLock() const {return lock;};
  };
  /// @}

  /****************************************//**
   * @name FRIENDS
   *******************************************/
  /// @{
  template<typename T>
  friend class Item;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  static QMap<QString, Node*> m_cmds;  ///< container
  static QMutex m_mutex;
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected:
  /// get item from map
  template<typename T>
  static Node &get ()
  {
    QMutexLocker l(&m_mutex);
    auto name = typeid (T).name();

    /* return, if present */
    if (m_cmds.contains (name))
      return *m_cmds[name];

    /* create & return */
    return *(m_cmds[name] = Node::create<T>());
  }
  /// @}

  /****************************************//**
   * @name DELETE
   *******************************************/
  /// @{
public:
  DapCmdFactory() = delete;
  DapCmdFactory(const DapCmdFactory&) = delete;
  DapCmdFactory(DapCmdFactory&&) = delete;
  ~DapCmdFactory() = delete;
  DapCmdFactory &operator=(const DapCmdFactory&) = delete;
  DapCmdFactory &operator=(DapCmdFactory&&) = delete;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDFACTORY_H
