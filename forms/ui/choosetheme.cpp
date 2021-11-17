/* INCLUDES */
#include "choosetheme.h"
#include "ui_choosetheme.h"
#include "dapguicommon.h"

#include <QTimer>
#include <QScroller>
#include <QJsonDocument>
#include <QJsonArray>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseTheme::ChooseTheme (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::ChooseTheme)
{
  ui->setupUi (this);
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

  /* setup model */
  auto theme = Common::fromFile ("://theme.json");
  ui->scrollArea->setColorThemeArray (QJsonDocument::fromJson (theme.toUtf8()).array(), this);

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

void ChooseTheme::slotSetColorTheme(const QString a_colorTheme)
{
  ui->scrollArea->setCurrentColorTheme (a_colorTheme);
}

/*-----------------------------------------*/
