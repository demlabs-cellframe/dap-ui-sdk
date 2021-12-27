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
  connect (ui->btnSwitch, &DapGuiSwitch::toggled,
           this, &Connection::sigSwitchToggle,
           Qt::QueuedConnection);
  connect (ui->btnSwitch, &DapGuiSwitch::clicked,
           this, &Connection::slotConnectionRequesteButtonPressed,
           Qt::QueuedConnection);
  connect (ui->btnServer, &DapGuiButton::clicked,
           this, &Connection::sigServerClicked,
           Qt::QueuedConnection);

  /* finish */
  slotSetDownUp (0, 0);
  setStatusIdicator(false);
}

Connection::~Connection()
{
  delete ui;
}

/********************************************
 * SLOTS
 *******************************************/

//void Connection::slotSwitchSetState(bool checked)
//{
//  ui->btnSwitch->setChecked (checked);
//  _slotUpdateStatusIcon();
//}

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
void Connection::setStatusText(QString a_text)
{
    ui->lTitle->setText(a_text);
}

void Connection::setServerInfo(QString a_name, QString a_ip)
{
    ui->btnServer->setMainText(a_name);
    ui->btnServer->setSubText(a_ip);
}

void Connection::setConnectedTime(QString a_text)
{
    ui->lUptime->setText(a_text);
}

void Connection::setStatusIdicator(bool a_enabled /*= false*/)
{
    ui->lStatusIconOn->setVisible (a_enabled);
    ui->lStatusIconOff->setVisible (!a_enabled);

//    ui->lStatusIcon->setChecked(a_authorized);
}

void Connection::setBtnSwitchChecked(bool a_authorized /*= true*/)
{
    ui->btnSwitch->setChecked(a_authorized);
}

void Connection::setDownloadSpeed(QString a_text)
{
    ui->lDownload->setMainText(a_text);
}

void Connection::setUploadSpeed(QString a_text)
{
    ui->lUpload->setMainText(a_text);
}

void Connection::slotSetStartedTime(QDateTime dt)
{
  m_started = dt;

  /* stop update timer */
  m_updateTime->stop();

  /* setup timer */
  if(!m_started.isNull())
    m_updateTime->start();
}


void Connection::_slotTimeUpdate()
{
  auto msecs  = m_started.msecsTo (QDateTime::currentDateTime());
  auto text   = UptimeStringHelper (msecs).asString();
  ui->lUptime->setText (text);
}

void Connection::slotConnectionRequesteButtonPressed()
{
    emit sigConnectionStatusChangeRequested();
}

//void Connection::slotUpdateStatusIcon(bool a_switch)
//{
//  ui->lStatusIconOn->setVisible (a_switch);
//  ui->lStatusIconOff->setVisible (!a_switch);
  
////    qDebug() << a_switch;
////  ui->lStatusIcon->setCssStyle(
////        a_switch
////        ? "conn-status-icon ic_online"
////        : "conn-status-icon ic_offline"
////      );
//}

/*-----------------------------------------*/
