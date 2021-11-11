/* INCLUDES */
#include "choosetheme.h"
#include "ui_choosetheme.h"

//#include "ui/model/choosethememodel.h"
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

ChooseTheme::ChooseTheme (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::ChooseTheme)
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

  /* setup model */
  ui->scrollArea->setModel (DapServersData::instance(), this);

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &ChooseTheme::sigReturn,
           Qt::QueuedConnection);
}

ChooseTheme::~ChooseTheme()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

ChooseThemeModel *ChooseTheme::model()
{
  return ui->scrollArea;
}

/********************************************
 * SLOTS
 *******************************************/

void ChooseTheme::slotRetranslated()
{
  ui->label->setText (tr ("Choose server"));
}

/*-----------------------------------------*/
