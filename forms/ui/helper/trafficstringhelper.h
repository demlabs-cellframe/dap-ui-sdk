#ifndef TRAFFICSTRINGHELPER_H
#define TRAFFICSTRINGHELPER_H

/* INCLUDES */
#include <QtCore>

/****************************************//**
 * @brief traffic string helper
 * @date 16.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class TrafficStringHelper
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  quint64 m_bytes;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  TrafficStringHelper (quint64 bytes);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
  QString asString();
  /// @}
};

/*-----------------------------------------*/
#endif // TRAFFICSTRINGHELPER_H
