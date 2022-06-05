#ifndef QSSMAP_H
#define QSSMAP_H

/* INCLUDES */
#include <QHash>
#include "qssitem.h"

class DapQmlStyle;

/* NAMESPACE */
namespace Style
{

/****************************************//**
 * @brief QSS style map
 *
 * Contains style items parsed from QSS file
 *
 * @ingroup groupDapGuiStyleParser
 * @date 05.06.2022
 * @author Mikhail Shilenko
 *******************************************/

class QssMap
{
  friend class ::DapQmlStyle;

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// fill map with provided stylesheet
  static void setup (const QString &styleSheet);

protected:
  /// get single item
  static const Style::QssItem *item (const QString &a_name);
  /// get items with names separated by space ' ' symbol
  static QList<const Style::QssItem*> items (const QString &a_names);
  /// get items with names
  static QList<const Style::QssItem*> items (const QStringList &a_names);
  /// @}
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // QSSMAP_H
