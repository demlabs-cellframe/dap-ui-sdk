/* INCLUDES */
#include "country.h"
#include "ui_country.h"
#include "dapguicommon.h"
#include <ui/helper/auxiliary/UiScaling.h>

#include <QTimer>
#include <QScroller>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMovie>
#include "DapDataLocal.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Country::Country (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Country)
{
  ui->setupUi (this);
//  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
//  ui->scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

#ifndef Q_OS_ANDROID
  ui->scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
#else // Q_OS_ANDROID
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
#endif // Q_OS_ANDROID

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
  ui->btnCountryFilter->setBtnStyle (DapGuiButton::EditTopMainBottomSub);

  connect (ui->btnCountryFilter, &DapGuiButton::textChanged,
           this, [=](QString text) {
      qDebug() << text;
      ui->scrollArea->setCountryList(
                  DapServersData::m_countryMap.keys().filter(
                      text, Qt::CaseInsensitive),
                  this);
  });

//  auto lay  = new QHBoxLayout;
//  lay->addWidget (m_spinner);
//  m_overlay->setLayout (lay);

  /* movie */
  //m_movie->setFileName(":/gui/asset/Spinner.gif");
  m_movie->setScaledSize (QSize (ww, ww));
  m_overlay->setVisible(false);

  /* setup model */
  ui->scrollArea->setCountryList(DapServersData::m_countryMap.keys(), this);

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &Country::sigReturn,
           Qt::QueuedConnection);
}

Country::~Country()
{
  delete ui;
  delete m_overlay;
  delete m_spinner;
  delete m_movie;
}

/********************************************
 * METHODS
 *******************************************/

CountryModel *Country::model()
{
  return ui->scrollArea;
}

void Country::showOverlay()
{
  int ww = m_spinner->width();
  m_spinner->move ((width() - ww) / 2, (height() - ww) / 2);
  m_overlay->resize (size());
  m_overlay->show();
  m_movie->start();
}

void Country::hideOverlay()
{
  m_overlay->hide();
  m_movie->stop();
}

/********************************************
 * SLOTS
 *******************************************/

void Country::slotRetranslated()
{
  ui->label->setText (tr ("Your country"));
  ui->scrollArea->slotRetranslate();
}

void Country::slotSetCountry(const QString a_country)
{
  ui->scrollArea->setCurrentCountry (a_country);
}

/*-----------------------------------------*/
