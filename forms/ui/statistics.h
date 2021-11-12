#ifndef STATISTICS_H
#define STATISTICS_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"
#ifndef NEUROMORPHIC
#include "../../ui/schedules.h"
#else // TestApp
#include "ui/helper/schedule/schedules.h"
#endif // TestApp

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Statistics; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for statistics display
 * @ingroup groupUiClasses
 * @date 27.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class Statistics : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Statistics *ui;
  Schedules schedules;

  quint64 m_downloadSpeed,
          m_uploadSpeed,
          m_bytesReceived,
          m_bytesSent,
          m_packetsReceived,
          m_packetsSent,
          m_ping;
  QDateTime m_started;
  QGraphicsScene *m_scene;
  QTimer *m_uptimeUpdateTimer;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  Statistics (QWidget *parent = nullptr);
  ~Statistics();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// start timer with random fill to graphic
  void startImitatingSchedules();
  void addItemGraphicSceneStyle (const QString &widget, const QString &styleWidget);

  quint64 downloadSpeed() const;
  void setDownloadSpeed (const quint64 &downloadSpeed);

  quint64 uploadSpeed() const;
  void setUploadSpeed (const quint64 &uploadSpeed);

  quint64 bytesReceived() const;
  void setBytesReceived (const quint64 &bytesReceived);
  void addBytesReceived (const quint64 &bytesReceived);

  quint64 bytesSent() const;
  void setBytesSent (const quint64 &bytesSent);
  void addBytesSent (const quint64 &bytesSent);

  quint64 packetsReceived() const;
  void setPacketsReceived (const quint64 &packetsReceived);
  void addPacketsReceived (const quint64 &packetsReceived);

  quint64 packetsSent() const;
  void setPacketsSent (const quint64 &packetsSent);
  void addPacketsSent (const quint64 &packetsSent);

  quint64 ping() const;
  void setPing (const quint64 &ping);

  quint64 uptime();
  QString uptimeStr();
  QDateTime started() const;
  void setStarted (const QDateTime &started);

  void updateGraph();
  void resetGraph();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void _slotUpdateUptimeTime();
  /// @}
};

/*-----------------------------------------*/
#endif // STATISTICS_H
