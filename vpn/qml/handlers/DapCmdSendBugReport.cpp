/* INCLUDES */

#include "DapCmdSendBugReport.h"
#include <QTimer>
#include <QRegularExpression>

/*-----------------------------------------*/
/* DEFS */

// this class controls the flag, that
// ignores next bugreport status message
class BugReportIgnoreCtl
{
  /* VARIABLES */
  QTimer _timer;
  bool _active;

  /* CONSTRUCT/DESTRUCT */
public:
  BugReportIgnoreCtl()
    : _active (false)
  {
    _timer.setSingleShot (true);
    _timer.setInterval (2000);
    QObject::connect (&_timer, &QTimer::timeout,
                      [this] { _active = false; });
  }

  /* METHODS */
  bool isActive() const { return _active; }
  void activate()
  {
    _active = true;
    _timer.start();
  }
  void deactivate()
  {
    _active = false;
    _timer.stop();
  }
};

/*-----------------------------------------*/
/* VARIABLES */

static BugReportIgnoreCtl *s_ignoreCtl  = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdSendBugReport::DapCmdSendBugReport (QObject *a_parent)
  : DapCmdClientAbstract (DapJsonCmdType::SEND_BUG_REPORT, a_parent)
{
  if (s_ignoreCtl == nullptr)
    s_ignoreCtl = new BugReportIgnoreCtl;
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdSendBugReport::sendBugReport(
  const QString &a_message,
  const QString &a_serial,
  const QString &attachFile,
  const QString &a_contactAddress)
{
  /* send report */
  QJsonObject obj =
  {
    {"attach_file", attachFile},
    {"contact_address", a_contactAddress},
    {"message", a_message},
    {"serial", a_serial},
  };
  sendCmd (&obj);
}

void DapCmdSendBugReport::cancelBugReport()
{
  s_ignoreCtl->activate();
}

void DapCmdSendBugReport::handleResult (const QJsonObject &a_result)
{
  if (s_ignoreCtl->isActive())
    return s_ignoreCtl->deactivate();

  QString request = a_result.value ("bugreport_answer").toString();

  qDebug() << "Bug report answer: " << request;

  if (request.contains ("saved successfully", Qt::CaseInsensitive))
    emit sigBugReportSent (request.remove (QRegularExpression ("\\D")));
  else
    emit sigBugReportSent ("");
}

void DapCmdSendBugReport::handleError (int, const QString &)
{
  qWarning() << *m_errorObject;
}

/*-----------------------------------------*/
