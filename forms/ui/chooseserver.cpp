/* INCLUDES */
#include "chooseserver.h"
#include "ui_chooseserver.h"

#include "ui/model/chooseservermodel.h"
#ifndef TestApp
#include "../core/DapServersData.h"
#else // TestApp
#include "dummy/DapServersData.h"
#endif // TestApp

#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseServer::ChooseServer (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::ChooseServer)
{
  ui->setupUi (this);

  /* simulate model items */
  auto list = DapServersData::instance();
  list->addServer (DapServerLocation::USA, "USA", "Address", 8081);
  list->addServer (DapServerLocation::RUSSIA, "Russia", "Address", 8081);
  list->addServer (DapServerLocation::GERMANY, "Gremany", "Address", 8081);

  /* setup model */
  ui->scrollArea->setModel (DapServersData::instance(), this);

  /* signals */
  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &ChooseServer::sigReturn,
           Qt::QueuedConnection);
}

ChooseServer::~ChooseServer()
{
  delete ui;
}

/*-----------------------------------------*/
