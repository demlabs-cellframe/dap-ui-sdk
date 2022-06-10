/* INCLUDES */
#include "chooseserver.h"
#include "ui_chooseserver.h"

#include "ui/model/chooseservermodel.h"
#include "../core/DapServersData.h"

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

  /* setup model */
  ui->scrollArea->setModel (DapServersData::instance(), this);

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &ChooseServer::sigReturn,
           Qt::QueuedConnection);
  btnReturnVisible(true);
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

void ChooseServer::setSelectedServer(QString name)
{
    ui->scrollArea->setSelectedItem(name);
}

void ChooseServer::btnReturnVisible(bool visible)
{
    // This function is needed because the btnReturn button is moved
    // when using widget blur.
    ui->btnReturn->setVisible(visible);
    ui->kelGuiWidget_Left->setVisible(!visible);
}

/*-----------------------------------------*/
