/* INCLUDES */
#include "DapCmdSystemTray.h"

#include <QMetaEnum>
#include <QJsonArray>

/* VARS */
static QMetaEnum *typeEnum        = nullptr;
static const QString actionParam  = "action";
static const QString actionValue  = "value";

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdSystemTray::DapCmdSystemTray(QObject *parent)
  : DapCmdServiceAbstract (DapJsonCmdType::CLIENT_INFO, parent)
{
  qRegisterMetaType<CdbMode> ("CdbMode");
  if (typeEnum == nullptr)
    typeEnum  = new QMetaEnum (QMetaEnum::fromType<RequestType> ());
}

DapCmdSystemTray::~DapCmdSystemTray()
{
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdSystemTray::handle (const QJsonObject *params)
{
  Q_UNUSED (params);

  if (params->contains (actionParam))
    {
      auto request  = params->value (actionParam).toString();
      auto type     = RequestType (typeEnum->keyToValue (request.toUtf8()));

      qDebug() << ("Request for show interface \"" + request + "\", type #%1").arg (int (type));

      switch (type)
        {
        case RequestType::show_window:                emit showGui();                 break;
        case RequestType::setting:                    emit showSettingInterface();    break;
        case RequestType::statistic:                  emit showStatisticInterface();  break;
        case RequestType::login:                      emit showLoginInterface();      break;
        case RequestType::dashboard:                  emit showDashboardInterface();  break;
        case RequestType::client_started:             emit clientFound();             break;
        case RequestType::client_exited:              emit clientClosed();            break;
        case RequestType::apllication_quit:           emit quitRequest();             break;
        case RequestType::tray_application_running:   emit trayApplicationFound();    break;
        case RequestType::change_server:
          {
            QJsonArray args = params->value (actionValue).toArray();
            emit changeServer (args[0].toString(), args[1].toString());
          } break;
        case RequestType::select_cdb_mode:
          {
            emit guiSelectCdbMode (CdbMode (params->value (actionValue).toInt()));
          } break;
        case RequestType::activation_changed:
          {
            QJsonArray args = params->value (actionValue).toArray();
            emit activationChanged (args[0].toBool(), args[1].toString());
          } break;
      }
  }
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdSystemTray::sendShowGui()
{
  sendGuiCmd (RequestType::show_window);
}

void DapCmdSystemTray::sendShowSettingInterface()
{
  sendGuiCmd (RequestType::setting);
}

void DapCmdSystemTray::sendShowStatisticInterface()
{
  sendGuiCmd (RequestType::statistic);
}

void DapCmdSystemTray::sendShowLoginInterface()
{
  sendGuiCmd (RequestType::login);
}

void DapCmdSystemTray::sendShowDashboardInterface()
{
  sendGuiCmd (RequestType::dashboard);
}

void DapCmdSystemTray::sendChangeServer (const QString &a_serverName, const QString &a_serverAddress)
{
  qDebug() << QString ("cmd sendChangeServer name[%1],ip[%2]").arg (a_serverName, a_serverAddress);
  auto name = typeEnum->valueToKey (int (RequestType::change_server));
  QJsonObject response = {
    {actionParam, name},
    {actionValue, QJsonArray {a_serverName, a_serverAddress}},
  };
  sendCmd (&response);
}

void DapCmdSystemTray::sendSelectCdbMode (CdbMode a_value)
{
  qDebug() << QString ("cmd sendSelectCdbMode mode[%1]").arg (int (a_value));
  auto name = typeEnum->valueToKey (int (RequestType::select_cdb_mode));
  QJsonObject response = {
    {actionParam, name},
    {actionValue, int (a_value)},
  };
  sendCmd (&response);
}

void DapCmdSystemTray::sendActiovationChange (bool a_activated, const QString &a_serialKey)
{
  qDebug() << QString ("cmd sendActiovationChange value [%1]").arg (a_activated);
  auto name = typeEnum->valueToKey (int (RequestType::activation_changed));
  QJsonObject response = {
    {actionParam, name},
    {actionValue, QJsonArray {a_activated, a_serialKey}},
  };
  sendCmd (&response);
}

void DapCmdSystemTray::apllicationQuitRequest()
{
  sendGuiCmd (RequestType::apllication_quit);
}

void DapCmdSystemTray::clientStarted()
{
  sendGuiCmd (RequestType::client_started);
}

void DapCmdSystemTray::clientExited()
{
  sendGuiCmd (RequestType::client_exited);
}

void DapCmdSystemTray::trayApplicationStarted()
{
  sendGuiCmd (RequestType::tray_application_running);
}



/// @param address Server address.
void DapCmdSystemTray::sendGuiCmd (const RequestType &a_type)
{
  auto name = typeEnum->valueToKey (int (a_type));
  qDebug() << "cmd sendGuiCmd " << name;

  QJsonObject response = {{actionParam, name}};
  sendCmd (&response);
}

/*-----------------------------------------*/
