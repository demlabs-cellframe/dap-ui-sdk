#ifndef STATISTICS_H
#define STATISTICS_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"
#ifndef NEUROMORPHIC
#include "../../ui/schedules.h"
#else // NEUROMORPHIC
#include "ui/helper/schedule/schedules.h"
#include "ui/helper/speedhelper.h"
#endif // NEUROMORPHIC

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
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QColor colorBackground    READ colorBackground    WRITE setColorBackground)
  Q_PROPERTY (QColor colorChartDownload READ colorChartDownload WRITE setColorChartDownload)
  Q_PROPERTY (QColor colorChartUpload   READ colorChartUpload   WRITE setColorChartUpload)
  /// @}

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
  QTimer *m_drawGraphTimer;
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
  void setDownloadSpeedString (const QString &downloadSpeed);

  quint64 uploadSpeed() const;
  void setUploadSpeed (const quint64 &uploadSpeed);
  void setUploadSpeedString (const QString &uploadSpeed);

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

  void resetGraph();

  const QColor &colorBackground() const;
  void setColorBackground(const QColor &newColorBackground);
  const QColor &colorChartDownload() const;
  void setColorChartDownload (const QColor &newColorChartDownload);
  const QColor &colorChartUpload() const;
  void setColorChartUpload (const QColor &newColorChartUpload);

private:
  void _slotUpdateUptimeTime();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotRetranslated();
  void updateGraph();
  /// @}
};

/*-----------------------------------------*/
#endif // STATISTICS_H
