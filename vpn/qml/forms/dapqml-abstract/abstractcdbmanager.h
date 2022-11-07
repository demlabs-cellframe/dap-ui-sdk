#ifndef ABSTRACTCDBMANAGER_H
#define ABSTRACTCDBMANAGER_H

/* INCLUDES */
#include <QtGlobal>
#include <QString>
#include <QList>

/****************************************//**
 * @brief abstract cdb manager
 * @ingroup groupUiModels
 * @date 17.10.2022
 * @author Mikhail Shilenko
 *******************************************/

class AbstractCdbManager
{
  Q_DISABLE_COPY_MOVE(AbstractCdbManager)

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  typedef QString                       Server;
  typedef QList<Server>::const_iterator ConstIterator;
  typedef QList<Server>::iterator       Iterator;
  /// @}

 /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  AbstractCdbManager() {}
  virtual ~AbstractCdbManager() {};

  /****************************************//**
   * @name VIRTUAL METHODS
   *******************************************/
  /// @{
public:
  virtual void append (const Server &a_server) = 0;
  virtual void append (Server &&a_server) = 0;
  virtual const Server &at (int a_index) const = 0;
  virtual void insert (int a_pos, const Server &a_server) = 0;
  virtual void insert (int a_pos, Server &&a_server) = 0;
  virtual void clear() = 0;
  virtual bool isEmpty() const = 0;
  virtual void move (int a_from, int a_to) = 0;
  virtual void removeAt (int a_pos) = 0;
  virtual bool removeOne (const QString &a_address) = 0;
  virtual void removeFirst() = 0;
  virtual void removeLast() = 0;

  virtual bool contains (const QString &a_address) const = 0;
  virtual ConstIterator cfind (const QString &a_address) const = 0;
  virtual Iterator find (const QString &a_address) = 0;
  virtual ConstIterator cindex (int a_pos) const = 0;
  virtual Iterator index (int a_pos) = 0;

  virtual ConstIterator cbegin() const = 0;
  virtual Iterator begin() = 0;
  virtual ConstIterator cend() const = 0;
  virtual Iterator end() = 0;
  virtual Iterator erase (Iterator a_pos) = 0;
  virtual Iterator erase (Iterator a_begin, Iterator a_end) = 0;
  virtual int size() const = 0;
  virtual Server value (int a_pos) = 0;

  virtual void update() = 0;
  virtual void doImport (const QString &a_filename) = 0;
  virtual void doExport (const QString &a_filename) const = 0;
  /// @}
};

/*-----------------------------------------*/
#endif // ABSTRACTCDBMANAGER_H
