/* INCLUDES */
#include "DapCmdFactory.h"
#include <QMutexLocker>

#include "DapCmdAndroidTunnel.h"
#include "DapCmdAuth.h"
#include "DapCmdConnect.h"
#include "DapCmdLastConnectionData.h"
#include "DapCmdNews.h"
#include "DapCmdPingServer.h"
#include "DapCmdSendBugReport.h"
#include "DapCmdServerData.h"
#include "DapCmdServersList.h"
#include "DapCmdSignUp.h"
#include "DapCmdStates.h"
#include "DapCmdStats.h"
#include "DapCmdTunTap.h"
#include "DapCmdUserData.h"

#ifdef BUILD_VAR_GOOGLE
#include "DapCmdPurchase.h"
#endif // BUILD_VAR_GOOGLE

/* VARS */
QMutex DapCmdFactory::m_mutex(QMutex::Recursive);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdFactory::DapCmdFactory()
{

}

DapCmdFactory::~DapCmdFactory()
{
  qDeleteAll(m_cmds);
}

/********************************************
 * STATIC METHODS
 *******************************************/

DapCmdFactory *DapCmdFactory::instance()
{
  static DapCmdFactory *__inst = nullptr;
  QMutexLocker l(&m_mutex);
  if (!__inst)
    __inst = new DapCmdFactory;
  return __inst;
}

/********************************************
 * PUBLIC OPERATORS
 *******************************************/

DapCmdServiceAbstract *DapCmdFactory::operator [] (const DapCmdFactory::Type &t)
{
  QMutexLocker l(&m_mutex);

  /* return, if present */
  if (m_cmds.contains (t))
    return m_cmds[t];

  /* create & return */
  switch (t)
    {
    case AndroidTunnel:       return m_cmds[t] = new DapCmdAndroidTunnel;
    case Auth:                return m_cmds[t] = new DapCmdAuth;
    case Connect:             return m_cmds[t] = new DapCmdConnect;
    case LastConnectionData:  return m_cmds[t] = new DapCmdLastConnectionData;
    case News:                return m_cmds[t] = new DapCmdNews;
    case PingServer:          return m_cmds[t] = new DapCmdPingServer;
    case SendBugReport:       return m_cmds[t] = new DapCmdSendBugReport;
    case ServerData:          return m_cmds[t] = new DapCmdServerData;
    case ServerList:          return m_cmds[t] = new DapCmdServersList;
    case SignUp:              return m_cmds[t] = new DapCmdSignUp;
    case States:              return m_cmds[t] = new DapCmdStates;
    case Stats:               return m_cmds[t] = new DapCmdStats;
    case TunTap:              return m_cmds[t] = new DapCmdTunTap;
    case UserData:            return m_cmds[t] = new DapCmdUserData;

    case Purchase:
#ifdef BUILD_VAR_GOOGLE
      return cmds[t] = new DapCmdPurchase;
#else // BUILD_VAR_GOOGLE
      break;
#endif // BUILD_VAR_GOOGLE
    }

  QString errorStr (QString ("%1 :: requested unknown type: %2")
                    .arg(__PRETTY_FUNCTION__)
                    .arg (t));
  qCritical() << errorStr;
  throw errorStr;
}

/*-----------------------------------------*/
