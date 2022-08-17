#ifndef QSSVALUE_H
#define QSSVALUE_H

/* INCLUDES */
#include <QString>
#include <QVariant>

/* NAMESPACE */
namespace DapStyle
{

/****************************************//**
 * @brief QSS style item value
 *
 * Pair of value name and value itself
 *
 * Used to invoke QObject::setProperty(name, value)
 *
 * @ingroup groupDapGuiStyleParser
 * @date 05.06.2022
 * @author Mikhail Shilenko
 *******************************************/

class QssValue
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  QString m_field;
  QString m_value;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  QssValue (const QString &a_src);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
  const QString &field() const;
  void setField (const QString &newField);

  const QString &value() const;
  void setValue (const QString &newValue);

  QVariant asVariant() const;

  /// parse qss body and split it into QssLine's instances (body inside '{}')
  static QList<QssValue> parse (const QString &a_src);
  /// @}
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // QSSVALUE_H
