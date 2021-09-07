#ifndef STYLESHEETCLASSMAP_H
#define STYLESHEETCLASSMAP_H

/* INCLUDES */
#include <QStringList>

/* NAMESPACE */
namespace StyleSheet
{

class ClassMap
{
  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  struct Item
  {
    QStringList keys;
    QStringList styles;
  };

  typedef QList<Item>::iterator Iterator;
  typedef QList<Item>::const_iterator ConstIterator;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QList<Item> m_items;
  QStringList m_allKeys;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  //ClassMap();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// remove all items and release memory
  void clear();
  /// store one style by it's class as a key
  void insert (const QString &key, const QString &style);
  /// store one style by it's classes list as a key
  void insert (const QStringList &keys, const QString &style);
  /// get list of all keys (classnames)
  const QStringList &keys() const;
  /// get list of all styles associated with provided key (classname)
  const QStringList &value (const QString &key) const;
  /// get list of all styles associated with provided keys (classnames)
  const QStringList &values (const QStringList &keys) const;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  /// @see value
  const QStringList &operator[] (const QString &key) const;
  /// @see values
  const QStringList &operator[] (const QStringList &keys) const;
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
public:
  Iterator find (const QString &key);
  ConstIterator find (const QString &key) const;
  bool isValid (Iterator &it) const;
  bool isValid (ConstIterator &it) const;
  /// @}
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // STYLESHEETCLASSMAP_H
