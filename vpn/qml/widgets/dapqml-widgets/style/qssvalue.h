#ifndef QSSVALUE_H
#define QSSVALUE_H

/* INCLUDES */
#include <QString>
#include <QVariant>

/* NAMESPACE */
namespace Style
{

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
