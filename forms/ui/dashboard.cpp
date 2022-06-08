/* INCLUDES */
#include "dashboard.h"
#include "ui_dashboard.h"
#include "helper/trafficstringhelper.h"
#include "helper/uptimestringhelper.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Dashboard::Dashboard (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Dashboard),
  m_started (QDateTime())
{
  /* setup ui */
  ui->setupUi (this);
  ui->btnSwitch->setEnabled(false);

  /* setup timer */
  m_updateTime = new QTimer;
  m_updateTime->setSingleShot (false);
  m_updateTime->setInterval (1000);
  _slotTimeUpdate();

  connect (m_updateTime, &QTimer::timeout,
           this, &Dashboard::_slotTimeUpdate,
           Qt::QueuedConnection);

  /* signals */
  connect (ui->btnSwitch, &DapGuiSwitch::toggled,
           this, &Dashboard::sigSwitchToggle,
           Qt::QueuedConnection);
  connect (ui->btnSwitch, &DapGuiSwitch::clicked,
           this, &Dashboard::slotConnectionRequesteButtonPressed,
           Qt::QueuedConnection);
  connect (ui->btnServer, &DapGuiButton::clicked,
           this, &Dashboard::sigServerClicked,
           Qt::QueuedConnection);

  ui->btnServer->setBtnStyle(DapGuiButton::TopSubBottomMain);

  /* finish */
  slotSetDownUp (0, 0);
  setStatusIdicator(false);
}

Dashboard::~Dashboard()
{
  delete ui;
}

/********************************************
 * SLOTS
 *******************************************/

/// @deprecated
void Dashboard::slotSetDownUp(quint64 down, quint64 up)
{
  slotSetDown (down);
  slotSetUp (up);
}

/// @deprecated
void Dashboard::slotSetDown(quint64 down)
{
  ui->lDownload->setMainText (TrafficStringHelper (down).asString());
}

/// @deprecated
void Dashboard::slotSetUp(quint64 up)
{
  ui->lUpload->setMainText (TrafficStringHelper (up).asString());
}

/// @deprecated
void Dashboard::slotSetSererIP(QString ip)
{
  ui->btnServer->setSubText (ip);
}

void Dashboard::setStatusText(QString a_text)
{
    ui->lTitle->setText(a_text);
}

void Dashboard::setServerInfo(QString a_name, QString a_ip)
{
    ui->btnServer->setMainText(a_name);
    ui->btnServer->setSubText(a_ip);
}

void Dashboard::setConnectedTime(QString a_text)
{
    ui->lUptime->setText(a_text);
}

void Dashboard::setStatusIdicator(bool a_enabled /*= false*/)
{
    ui->lStatusIconOn->setVisible (a_enabled);
#ifndef DISABLE_CLOCK_WHEN_DISCONNECTED
    ui->lStatusIconOff->setVisible (!a_enabled);
#else
    ui->lStatusIconOff->setVisible (false);
    ui->lUptime->setVisible(a_enabled);
    if (!a_enabled)
        ui->lTitle->setCssStyle("conn-top-text-connected font24 darkblue normalbold lato noborder nobackground");
    else
        ui->lTitle->setCssStyle("conn-top-text-connected font24 red normalbold lato noborder nobackground");
#endif

//    ui->lStatusIcon->setChecked(a_authorized);
}

void Dashboard::setBtnSwitchChecked(bool a_authorized /*= true*/)
{
    ui->btnSwitch->setChecked(a_authorized);
}

void Dashboard::setDownloadSpeed(QString a_text)
{
    ui->lDownload->setMainText(a_text);
}

void Dashboard::setUploadSpeed(QString a_text)
{
    ui->lUpload->setMainText(a_text);
}

/// @deprecated
void Dashboard::slotSetStartedTime(QDateTime dt)
{
  m_started = dt;

  /* stop update timer */
  m_updateTime->stop();

  /* setup timer */
  if(!m_started.isNull())
    m_updateTime->start();
}

/// @deprecated
void Dashboard::_slotTimeUpdate()
{
  auto msecs  = m_started.msecsTo (QDateTime::currentDateTime());
  auto text   = UptimeStringHelper (msecs).asString();
  ui->lUptime->setText (text);
}

void Dashboard::slotRetranslated()
{
  ui->lDownload->setSubText (tr ("download"));
  ui->lUpload->setSubText (tr ("upload"));
}
void Dashboard::slotConnectionRequesteButtonPressed()
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

//void Dashboard::slotErrorText (QString text, ErrorColor color)
void Dashboard::slotErrorText (QString text)
{
  ui->lStatus->setText (text);
  ui->lStatus->setCssStyle ("font13 red_error lato normal");
}

void Dashboard::setBtnSwitchEnabled(bool enabled)
{
    ui->btnSwitch->setEnabled(enabled);
}

/*-----------------------------------------*/
