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
}

SplashScreen::~SplashScreen()
{
  delete ui;
}

/*-----------------------------------------*/
