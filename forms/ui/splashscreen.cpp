/* INCLUDES */
#include "splashscreen.h"
#include "ui_splashscreen.h"
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

SplashScreen::SplashScreen (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::SplashScreen)
{
  ui->setupUi (this);
  ui->pLoading->setVisible (false);

  auto version  = QString ("Version %1\n%2")
      .arg (DAP_VERSION, __DATE__);
#ifdef FOOTER_LABEL_POWERED_BY
  version += "\nPowered by Cellframe Network";
#endif

//  auto lay  = ui->MainWidget->layout();
//  for (auto i = 0, e = lay->count(); i < e; i++)
//    {
//      auto item   = lay->itemAt(i);
//      auto widget = item->widget();
//      auto spacer = item->spacerItem();
//      auto size   = item->sizeHint();

//      if (widget)
//        {
//          qDebug() << __FUNCTION__ << i << widget->objectName()
//                   << ":" << widget->size() << size;
//          continue;
//        }

//      if (spacer)
//        {
//          qDebug() << __FUNCTION__ << "spacer at:" << i
//                   << ":" << size;
//          continue;
//        }

//        qDebug() << __FUNCTION__ << "unknown item at:" << i
//                 << ":" << size;
//    }

  ui->lVersion->setText (version);
}

SplashScreen::~SplashScreen()
{
  delete ui;
}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void SplashScreen::setState (ConnectionState a_state)
{
  QString statusText = (a_state == ConnectionState::ServersListLoading) ? tr ("Loading servers list...") : tr ("Connecting to service...");
  ui->lStatus->setText (statusText);
}

void SplashScreen::setErrorMessage (const QString &msg)
{
  ui->lStatus->setText (msg);
}

/*-----------------------------------------*/
