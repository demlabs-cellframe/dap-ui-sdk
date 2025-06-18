#ifndef DAPCMDGEOIP_H
#define DAPCMDGEOIP_H

/* INCLUDES */

#include "DapCmdServiceAbstract.h"
#include <QObject>

/****************************************//**
 * @brief get GeoIP data
 * @date 10.07.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapCmdGeoIP : public DapCmdServiceAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdGeoIP (QObject *a_parent = nullptr);
  virtual ~DapCmdGeoIP() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  /// Process command.
  /// @param params Command parameters.
  void handle (const QJsonObject *params) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigGetGeoIP();
  void sigSendGeoIP (QString a_value);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDGEOIP_H
