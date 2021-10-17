/* INCLUDES */
#include "settings.h"
#include "ui_settings.h"

#include "ui/model/settingsmodel.h"
#include <QTimer>
#include <QScroller>

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
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

#ifndef Q_OS_ANDROID
  ui->scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
#endif // Q_OS_ANDROID

  /* signals */
  connect (this, &Settings::sigSetDaysLeft,
           ui->scrollArea, &SettingsModel::slotSetDaysLeft,
           Qt::QueuedConnection);
  connect (this, &Settings::sigResetDaysLeft,
           ui->scrollArea, &SettingsModel::slotResetDaysLeft,
           Qt::QueuedConnection);

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
