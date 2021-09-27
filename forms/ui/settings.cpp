/* INCLUDES */
#include "settings.h"
#include "ui_settings.h"

#include "ui/model/settingsmodel.h"
#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Settings::Settings (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Settings)
{
  /* setup ui */
  ui->setupUi (this);
  ui->scrollArea->setInterface (this); // this will allow to send signals

  /* signals */
  connect (this, &Settings::sigSetDaysLeft,
           ui->scrollArea, &SettingsModel::slotSetDaysLeft);

  /* start model */
  ui->scrollArea->slotSetup();
}

Settings::~Settings()
{
  delete ui;
}

/********************************************
 * SLOTS
 *******************************************/

void Settings::slotSetVersion(const QString &a_text)
{
  ui->scrollArea->setVersionText (a_text);
}

/*-----------------------------------------*/
