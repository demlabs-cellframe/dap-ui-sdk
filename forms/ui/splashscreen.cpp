/* INCLUDES */
#include "splashscreen.h"
#include "ui_splashscreen.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

SplashScreen::SplashScreen(QWidget *parent) :
  QWidget(parent),
  m_ui(new Ui::SplashScreen)
{
  m_ui->setupUi(this);
}

SplashScreen::~SplashScreen()
{
  delete m_ui;
}

/*-----------------------------------------*/



void SplashScreen::setState(ConnectionState a_state)
{
    QString statusText = (a_state == ConnectionState::ServersListLoading) ? tr("Loading servers list...") : tr("Connecting to service...");
    m_ui->lStatus->setText(statusText);
}

void SplashScreen::setErrorMessage(const QString &msg)
{
    m_ui->lStatus->setText(msg);
}
