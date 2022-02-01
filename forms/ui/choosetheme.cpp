/* INCLUDES */
#include "choosetheme.h"
#include "ui_choosetheme.h"
#include "dapguicommon.h"
#include <ui/helper/auxiliary/UiScaling.h>

#include <QTimer>
#include <QScroller>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMovie>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseTheme::ChooseTheme (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::ChooseTheme)
{
  ui->setupUi (this);
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

  /* setup overlay */
  int ww = UiScaling::pointsToPixels (160, UiScaling::getNativDPI());
  m_overlay = new QWidget (this);
  m_overlay->setObjectName ("CTOVER");
  m_overlay->resize (size());
  m_overlay->setStyleSheet ("#CTOVER {background-color: rgba(0,0,0,0.5);}");

  m_spinner = new QLabel (m_overlay);
  m_spinner->resize (ww, ww);
  m_spinner->setMinimumSize (ww, ww);
  m_spinner->setMaximumSize (ww, ww);
  m_spinner->setScaledContents (true);
  // m_spinner->setCssStyle ("ic_spinner bugrep-spinner noborder nobackground");
  m_spinner->setStyleSheet(
        "background-color: rgba(0,0,0,0);"
        "border: none;");

  m_movie   = new QMovie(":/gui/asset/Spinner.gif");
  m_spinner->setMovie (m_movie);

//  auto lay  = new QHBoxLayout;
//  lay->addWidget (m_spinner);
//  m_overlay->setLayout (lay);

  /* movie */
  //m_movie->setFileName(":/gui/asset/Spinner.gif");
  m_movie->setScaledSize (QSize (ww, ww));
  m_overlay->setVisible(false);

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
  delete m_overlay;
  delete m_spinner;
  delete m_movie;
}

/********************************************
 * METHODS
 *******************************************/

ChooseThemeModel *ChooseTheme::model()
{
  return ui->scrollArea;
}

void ChooseTheme::showOverlay()
{
  int ww = m_spinner->width();
  m_spinner->move ((width() - ww) / 2, (height() - ww) / 2);
  m_overlay->resize (size());
  m_overlay->show();
  m_movie->start();
}

void ChooseTheme::hideOverlay()
{
  m_overlay->hide();
  m_movie->stop();
}

/********************************************
 * SLOTS
 *******************************************/

void ChooseTheme::slotRetranslated()
{
  ui->label->setText (tr ("Choose theme"));
  ui->scrollArea->slotRetranslate();
}

void ChooseTheme::slotSetColorTheme(const QString a_colorTheme)
{
  ui->scrollArea->setCurrentColorTheme (a_colorTheme);
}

/*-----------------------------------------*/
