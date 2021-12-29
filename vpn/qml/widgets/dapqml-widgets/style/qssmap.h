#ifndef QSSMAP_H
#define QSSMAP_H

/* INCLUDES */
#include <QHash>
#include "qssitem.h"

class DapQmlStyle;

/* NAMESPACE */
namespace Style
{

class QssMap
{
  friend class ::DapQmlStyle;

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  static QHash<QString, Style::QssItem> m_map;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// fill map with provided stylesheet
  static void setup (const QString &styleSheet);
  /// apply style patch
  static void patch (const QJsonArray &array, const QString &themeDir);

protected:
  /// get single item
  static const Style::QssItem &item (const QString &a_name);
  /// get items with names separated by space ' ' symbol
  static QList<const Style::QssItem&> &items (const QString &a_names);
  /// get items with names
  static QList<const Style::QssItem&> &items (const QStringList &a_names);
  /// @}
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // QSSMAP_H
