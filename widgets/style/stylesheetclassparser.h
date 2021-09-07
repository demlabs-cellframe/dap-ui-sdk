#ifndef STYLESHEETCLASSPARSER_H
#define STYLESHEETCLASSPARSER_H

/* INCLUDES */
#include <QStringList>
#include "stylesheetclassmap.h"

/* NAMESPACE */
namespace StyleSheet
{

/****************************************//**
 * @brief style sheet parser for classes
 * Class takes stylesheet string, find
 * all names (starting with dots) and
 * stores style body into map.
 * @see KelGuiStyleManager
 * @ingroup groupKelGuiStyle
 * @date 07.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ClassParser
{
  friend class KelGuiStyleManager;

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
  typedef QString::const_iterator it;
  typedef void (*callback) (ClassParser *ssc, it &i);
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  ClassMap &styleMap;
  QStringList names;
  QString currentName, currentBody;
  callback method;
  int index; const float &DPI;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  ClassParser (
    const QString &styleSheet,
    ClassMap &styleMap,
    const float &DPI);
  /// @}

  /****************************************//**
   * @name STATIC METHODS
   *******************************************/
  /// @{
protected:
  static void none (ClassParser *ssc, it &i);
  static void naming (ClassParser *ssc, it &i);
  static void body (ClassParser *ssc, it &i);
  /// @}
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // STYLESHEETCLASSPARSER_H
