/* INCLUDES */
#include "connection.h"
#include "ui_connection.h"
#include "helper/trafficstringhelper.h"
#include "helper/uptimestringhelper.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Connection::Connection (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Connection),
  m_started (QDateTime())
{
  /* setup ui */
  ui->setupUi (this);

  /* setup timer */
  m_updateTime = new QTimer;
  m_updateTime->setSingleShot (false);
  m_updateTime->setInterval (1000);
  _slotTimeUpdate();

  connect (m_updateTime, &QTimer::timeout,
           this, &Connection::_slotTimeUpdate,
           Qt::QueuedConnection);

  /* signals */
  connect (ui->btnSwitch, &KelGuiPushButton::toggled,
           this, &Connection::sigSwitchToggle,
           Qt::QueuedConnection);
  connect (ui->btnSwitch, &KelGuiPushButton::clicked,
           this, &Connection::_slotUpdateStatusIcon,
           Qt::QueuedConnection);
  connect (ui->btnServer, &KelGuiButton::clicked,
           this, &Connection::sigServerClicked,
           Qt::QueuedConnection);

  /* finish */
  _slotUpdateStatusIcon();
}

Connection::~Connection()
{
  delete ui;
}

/********************************************
 * SLOTS
 *******************************************/

void Connection::slotSwitchSetState(bool checked)
{
  ui->btnSwitch->setChecked (checked);
  _slotUpdateStatusIcon();
}

void Connection::slotSetDownUp(quint64 down, quint64 up)
{
  slotSetDown (down);
  slotSetUp (up);
}

void Connection::slotSetDown(quint64 down)
{
  ui->lDownload->setMainText (TrafficStringHelper (down).asString());
}

void Connection::slotSetUp(quint64 up)
{
  ui->lUpload->setMainText (TrafficStringHelper (up).asString());
}

void Connection::slotSetSererIP(QString ip)
{
  ui->btnServer->setSubText (ip);
}

void Connection::slotSetStartedTime(QDateTime dt)
{
  m_started = dt;

  /* stop update timer */
  m_updateTime->stop();

  /* setup timer */
  if(!m_started.isNull())
    m_updateTime->start();

  _slotUpdateStatusIcon();
}

void Connection::_slotTimeUpdate()
{
  auto msecs  = m_started.msecsTo (QDateTime::currentDateTime());
  auto text   = UptimeStringHelper (msecs).asString();
  ui->lUptime->setText (text);
}

void Connection::_slotUpdateStatusIcon()
{
  ui->lStatusIcon->setCssStyle(
        (!m_started.isNull())
        ? "conn-status-icon ic_online"
        : "conn-status-icon ic_offline"
      );
}

/*-----------------------------------------*/
