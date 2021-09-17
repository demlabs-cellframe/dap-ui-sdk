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

  /* start model */
  QTimer::singleShot (0, ui->scrollArea, &SettingsModel::slotSetup);
}

Settings::~Settings()
{
  delete ui;
}

/*-----------------------------------------*/
