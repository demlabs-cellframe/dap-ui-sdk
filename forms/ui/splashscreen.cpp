/* INCLUDES */
#include "splashscreen.h"
#include "ui_splashscreen.h"

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
