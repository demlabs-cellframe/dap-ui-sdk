/* INCLUDES */
#include "statistics.h"
#include "ui_statistics.h"
#include <QTimer>

/* VARS */
static DapGraphicSceneStyle graphicSceneStyle;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Statistics::Statistics (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Statistics)
{
  ui->setupUi (this);
}

Statistics::~Statistics()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

void Statistics::startImitatingSchedules()
{
  /* VARS */
  static QTimer *t              = nullptr;
  static QGraphicsScene *scene  = nullptr;

  if (t)
    return;

  scene = new QGraphicsScene;
  ui->graphicsView->setScene (scene);

  t = new QTimer;
  t->setSingleShot (false);
  t->setInterval (1000);

  // Set styles for graphics.
//  addItemGraphicSceneStyle ("shChartDownload",   AppStyleSheetHandler::getWidgetStyleSheet ("#shChartDownload", "active"));
//  addItemGraphicSceneStyle ("shChartUpload",     AppStyleSheetHandler::getWidgetStyleSheet ("#shChartUpload", "active"));
//  addItemGraphicSceneStyle ("shGrid",            AppStyleSheetHandler::getWidgetStyleSheet ("#shGrid", "active"));
//  addItemGraphicSceneStyle ("graphicsLines",     AppStyleSheetHandler::getWidgetStyleSheet ("#graphicsLines", "active"));
  addItemGraphicSceneStyle ("shChartDownload",  "color:#D61F5D;depth:1;background-color:rgba(0,181,43,0.00);");
  addItemGraphicSceneStyle ("shChartUpload",    "color:#2D1FD6;depth:1;background-color:rgba(3,167,198,0.00);");
  addItemGraphicSceneStyle ("shGrid",           "color:#ECECEC;depth:0.1;");
  addItemGraphicSceneStyle ("graphicsLines",    "horizontalLines:19;verticalLines:8;");//"horizontalLines:19;verticalLines:8;sceneWidth:236px;sceneHeight:119px;");

  connect (t, &QTimer::timeout, [&]
  {
    auto rec  = qrand() % 1024,
    sent = qrand() % 1024;
    schedules.addInp (rec);
    schedules.addOut (sent);

    schedules.draw_chart (scene);
    scene->update();
  });

  t->start();
}

void Statistics::addItemGraphicSceneStyle (const QString &widget, const QString &styleWidget)
{
  QStringList strParametrs = styleWidget.split (";");
  strParametrs.removeAll ("");
  QMap<QString, QVariant> param;
  for (QString str : strParametrs)
    {
      QStringList tempParam = str.split (":");
      param.insert (tempParam.at (0), tempParam.at (1));
    }
  graphicSceneStyle.insert (widget, param);

  schedules.setStyle (graphicSceneStyle);
}

quint64 Statistics::downloadSpeed() const
{
  return m_downloadSpeed;
}

void Statistics::setDownloadSpeed (const quint64 &downloadSpeed)
{
  m_downloadSpeed = downloadSpeed;
  auto text       = QString ("%1 Mbps").arg (m_downloadSpeed);
  ui->statDownSp->setMainText (text);
}

quint64 Statistics::uploadSpeed() const
{
  return m_uploadSpeed;
}

void Statistics::setUploadSpeed (const quint64 &uploadSpeed)
{
  m_uploadSpeed = uploadSpeed;
  auto text       = QString ("%1 Mbps").arg (m_uploadSpeed);
  ui->statUpSp->setMainText (text);
}

quint64 Statistics::bytesReceived() const
{
  return m_bytesReceived;
}

void Statistics::setBytesReceived (const quint64 &bytesReceived)
{
  m_bytesReceived = bytesReceived;
  auto text       = QString ("%1 Bytes").arg (m_bytesReceived);
  ui->statBytesRec->setMainText (text);
}

void Statistics::addBytesReceived (const quint64 &bytesReceived)
{
  m_bytesReceived += bytesReceived;
  auto text       = QString ("%1 Bytes").arg (m_bytesReceived);
  ui->statBytesRec->setMainText (text);
}

quint64 Statistics::bytesSent() const
{
  return m_bytesSent;
}

void Statistics::setBytesSent (const quint64 &bytesSent)
{
  m_bytesSent = bytesSent;
  auto text   = QString ("%1 Bytes").arg (m_bytesSent);
  ui->statBytesSent->setMainText (text);
}

void Statistics::addBytesSent (const quint64 &bytesSent)
{
  m_bytesSent += bytesSent;
  auto text   = QString ("%1 Bytes").arg (m_bytesSent);
  ui->statBytesSent->setMainText (text);
}

quint64 Statistics::packetsReceived() const
{
  return m_packetsReceived;
}

void Statistics::setPacketsReceived (const quint64 &packetsReceived)
{
  m_packetsReceived = packetsReceived;
  auto text         = QString::number (m_packetsReceived);
  ui->statPackRec->setMainText (text);
}

void Statistics::addPacketsReceived (const quint64 &packetsReceived)
{
  m_packetsReceived += packetsReceived;
  auto text         = QString::number (m_packetsReceived);
  ui->statPackRec->setMainText (text);
}

quint64 Statistics::packetsSent() const
{
  return m_packetsSent;
}

void Statistics::setPacketsSent (const quint64 &packetsSent)
{
  m_packetsSent = packetsSent;
  auto text         = QString::number (m_packetsSent);
  ui->statPackSent->setMainText (text);
}

void Statistics::addPacketsSent (const quint64 &packetsSent)
{
  m_packetsSent += packetsSent;
  auto text         = QString::number (m_packetsSent);
  ui->statPackSent->setMainText (text);
}

quint64 Statistics::ping() const
{
  return m_ping;
}

void Statistics::setPing (const quint64 &ping)
{
  m_ping    = ping;
  auto text = QString ("%1 ms").arg (m_ping);
  ui->statPing->setMainText (text);
}

QDateTime Statistics::uptime()
{
//  auto secs = m_started.secsTo (QDateTime::currentDateTime());
//  QDateTime result;
//  result.addSecs (secs);
//  return result;
}

QString Statistics::uptimeStr()
{

}

QDateTime Statistics::started() const
{
  return m_started;
}

void Statistics::setStarted (const QDateTime &started)
{
  m_started = started;
}

/*-----------------------------------------*/
