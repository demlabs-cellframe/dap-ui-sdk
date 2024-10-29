#ifndef UPTIMESTRINGHELPER_H
#define UPTIMESTRINGHELPER_H

/* INCLUDES */
#include <QtCore>

/****************************************//**
 * @brief uptime string helper
 * @date 16.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class UptimeStringHelper
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  quint64 m_msecs;
  quint16 sec, min, hour, day;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  UptimeStringHelper (quint64 msec);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString asString();
  /// @}
};

/*-----------------------------------------*/
#endif // UPTIMESTRINGHELPER_H
