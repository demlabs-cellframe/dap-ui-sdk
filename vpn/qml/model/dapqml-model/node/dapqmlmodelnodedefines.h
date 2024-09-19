#ifndef DAPQMLMODELNODEDEFINES_H
#define DAPQMLMODELNODEDEFINES_H

/* INCLUDES */

//#include <qtypes.h>
#include <QString>
#include <QHash>

/* NAMESPACE */

/****************************************//**
 * @brief node models namespace
 * @ingroup groupUiModels
 * @date 26.02.2024
 * @author Mikhail Shilenko
 *******************************************/

namespace DapQmlModelNode {

/* DEFINES */

/****************************************//**
 * @brief node models fields
 * @ingroup groupUiModels
 * @date 26.02.2024
 * @author Mikhail Shilenko
 *******************************************/

enum class FieldId : qint32
{
  /* order flields */
  location,
  node_addr,
  price,
  priceShort,
  units,
  units_value,
  server,
  hash,
  ipAddress,

  /* name + value fields */
  name,
  value,
  misc,

  /* model fields */
  network,
  wallet,
};

/****************************************//**
 * @brief node models order item
 * @ingroup groupUiModels
 * @date 26.02.2024
 * @author Mikhail Shilenko
 *******************************************/

struct OrderItem
{
  QString location;
  QString price;
  QString units;
  QString units_value;
  QString server;
  QString node_addr;
  QString hash;
  QString ipAddress;
};

/****************************************//**
 * @brief node models name and value item
 * @ingroup groupUiModels
 * @date 26.02.2024
 * @author Mikhail Shilenko
 *******************************************/

struct NameValueItem
{
  QString name;
  QString value;
};

/* VARIABLES */

extern QHash<int, QByteArray> nodeFieldsMap;

/*-----------------------------------------*/
}; // DapQmlModelNode
/*-----------------------------------------*/
#endif // DAPQMLMODELNODEDEFINES_H
