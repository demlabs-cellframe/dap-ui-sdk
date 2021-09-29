/* INCLUDES */
#include "versionupdate.h"
#include "ui_versionupdate.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

VersionUpdate::VersionUpdate(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::VersionUpdate)
{
  ui->setupUi(this);

  /* signals */
  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &VersionUpdate::sigReturn,
           Qt::QueuedConnection);
  connect (ui->btnUpdate, &KelGuiPushButton::clicked,
           this, &VersionUpdate::sigUpdate,
           Qt::QueuedConnection);
}

VersionUpdate::~VersionUpdate()
{
  delete ui;
}

/*-----------------------------------------*/
