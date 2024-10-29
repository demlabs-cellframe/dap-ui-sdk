#ifndef QSSITEM_H
#define QSSITEM_H

/* INCLUDES */
#include <QHash>
#include <QString>
#include <QVariant>

/* NAMESPACE */
namespace DapStyle
{

/****************************************//**
 * @name DEFS
 *******************************************/
/// @{
typedef QHash<QString, QVariant> QssItemMap;
/// @}

/****************************************//**
 * @brief QSS style item
 *
 * Represents style scope and provides
 * it's values
 *
 * @ingroup groupDapGuiStyleParser
 * @date 05.06.2022
 * @author Mikhail Shilenko
 *******************************************/

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
