#ifndef QSSLINK_H
#define QSSLINK_H

/* INCLUDES */
#include <QHash>
#include <QVariant>
#include "qssitem.h"

/* NAMESPACE */
namespace DapStyle
{

/****************************************//**
 * @brief QSS style link item
 *
 * Represents link scope and provides
 * it's values
 *
 * Used for themes.
 *
 * Link scope starts with \@ symbol (example: .@dark{} .@light{})
 *
 * @ingroup groupDapGuiStyleParser
 * @date 05.06.2022
 * @author Mikhail Shilenko
 *******************************************/

class QssLink
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  QHash<QString, QVariant> m_map;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  QssLink();
  QssLink (const QssLink &a_src);
  QssLink (QssLink &&a_src);
  ~QssLink();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static QStringList categories();
  static QString currentCategory();
  static void changeCategory (const QString a_category);
  static QVariant get (QString a_name, QString a_category = "");
  static void set (QString a_name, QString a_category, const QVariant &a_value);
  static void set (const QString &a_category, const QssItem &a_value);
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  QssLink &operator = (const QssLink &a_src);
  QssLink &operator = (QssLink &&a_src);
  /// @}
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // QSSLINK_H
