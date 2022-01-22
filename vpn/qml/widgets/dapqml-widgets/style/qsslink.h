#ifndef QSSLINK_H
#define QSSLINK_H

/* INCLUDES */
#include <QHash>
#include <QVariant>
#include "qssitem.h"

/* NAMESPACE */
namespace Style
{

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
  static void changeCategory (const QString a_category);
  static QVariant get (QString a_name, QString a_category = "");
  static void set (const QString &a_name, QString a_category, const QVariant &a_value);
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
