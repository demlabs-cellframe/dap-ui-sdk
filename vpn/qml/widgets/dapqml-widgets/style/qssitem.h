#ifndef QSSITEM_H
#define QSSITEM_H

/* INCLUDES */
#include <QHash>
#include <QString>
#include <QVariant>

/* NAMESPACE */
namespace Style
{

/****************************************//**
 * @name DEFS
 *******************************************/
/// @{
typedef QHash<QString, QVariant> QssItemMap;
/// @}

class QssItem
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QssItemMap m_map;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QssItemMap::iterator insert (const QString &a_key, const QVariant &a_value);
  bool remove (const QString &a_key);
  QssItemMap::iterator erase (QssItemMap::iterator &it);

  QssItemMap::iterator begin();
  QssItemMap::iterator end();
  QssItemMap::const_iterator cbegin() const;
  QssItemMap::const_iterator cend() const;

  int size() const;

  QList<QString> keys() const;
  QList<QVariant> values() const;

  QVariant value (const QString &a_key) const;
  QString key (const QVariant &a_value) const;
  /// @}
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // QSSITEM_H
