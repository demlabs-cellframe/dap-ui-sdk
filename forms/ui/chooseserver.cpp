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
#include <QScroller>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseServer::ChooseServer (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::ChooseServer)
{
  ui->setupUi (this);
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

#ifdef TestApp
  /* simulate model items */
  auto list = DapServersData::instance();
  list->addServer (DapServerLocation::USA, "USA", "Address", 8081);
  list->addServer (DapServerLocation::RUSSIA, "Russia", "Address", 8081);
  list->addServer (DapServerLocation::GERMANY, "Gremany", "Address", 8081);
#endif // TestApp

#ifdef NEUROMORPHIC
  /* setup model */
  ui->scrollArea->setModel (DapServersData::instance(), this);
#endif // NEUROMORPHIC

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &ChooseServer::sigReturn,
           Qt::QueuedConnection);
}

ChooseServer::~ChooseServer()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

ChooseServerModel *ChooseServer::model()
{
  return ui->scrollArea;
}

/********************************************
 * SLOTS
 *******************************************/

void ChooseServer::slotRetranslated()
{
  ui->label->setText (tr ("Choose server"));
}

/*-----------------------------------------*/
