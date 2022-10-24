/* INCLUDES */
#include "popupdialog.h"
#include "ui_popupdialog.h"
#include <ui/helper/auxiliary/UiScaling.h>
#include <QDebug>
#include <QGraphicsBlurEffect>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

PopupDialog::PopupDialog (QWidget *parent) :
  QWidget (parent),
  ui (new Ui::PopupDialog),
  m_parent (nullptr),
  m_shadow (new QWidget)
{
  ui->setupUi (this);

  /* get size */
  auto ww = UiScaling::pointsToPixels (280, UiScaling::getNativDPI()),
       hh = UiScaling::pointsToPixels (186, UiScaling::getNativDPI());

  QSize sz (ww,hh);
  setFixedSize (sz);
  setMinimumSize (sz);
  setMaximumSize (sz);
  defaultHeight     = hh;
  defaultTextHeight = ui->lDescription->height();

  qDebug() << "PopupDialog::Construct size: " << sz;


  connect (ui->btnYes, &DapGuiLabel::clicked,
           this, &PopupDialog::sigYes,
           Qt::QueuedConnection);
  connect (ui->btnNo, &DapGuiLabel::clicked,
           this, &PopupDialog::sigNo,
           Qt::QueuedConnection);
  connect (ui->btnYes, &DapGuiLabel::clicked,
           this, &PopupDialog::sigOk,
           Qt::QueuedConnection);

  connect (ui->btnYes, &DapGuiLabel::clicked,
           this, &PopupDialog::_slotButtonPress,
           Qt::QueuedConnection);
  connect (ui->btnNo, &DapGuiLabel::clicked,
           this, &PopupDialog::_slotButtonPress,
           Qt::QueuedConnection);

  m_shadow->setStyleSheet (
    "background-color: rgba(0,0,0,0.5);"
  );
}

PopupDialog::~PopupDialog()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

void PopupDialog::_adjustHeight (const QString &a_decription)
{
  /* get item boundingRect, original font size and name */
  auto fontSize = ui->lDescription->font().pointSize();
  auto fontRect = QRect(
      ui->lDescription->property ("x").toInt(),
      ui->lDescription->property ("y").toInt(),
      ui->lDescription->property ("width").toInt(),
      ui->lDescription->property ("height").toInt()
    );
  auto fontName = ui->lDescription->font().family();

  /* setup font metrics */
  QFont font (fontName, fontSize);
  QFontMetrics fm (font);

  /* get actual metrics */
  auto rect = fm.boundingRect(
    fontRect,
    Qt::AlignVCenter | Qt::AlignHCenter,
    a_decription);

  /* calc and store result */
  auto metricsHeight  =
      (rect.height() > fontRect.height())
      ? (fontRect.height())
      : (0);
  setMinimumHeight (defaultHeight + metricsHeight);
  setMaximumHeight (defaultHeight + metricsHeight);
}

/********************************************
 * SLOTS
 *******************************************/

void PopupDialog::slotShow (
    QWidget *a_parent,
    const QString &a_title,
    const QString &a_description,
    const QString &a_btnYes,
    const QString &a_btnNo /*= ""*/)
{
  /* create blur effect */
  QGraphicsBlurEffect *p_blur = new QGraphicsBlurEffect;
  p_blur->setBlurRadius (5);
  p_blur->setBlurHints (QGraphicsBlurEffect::PerformanceHint);

  /* setup effect and disable parent */
  m_parent  = a_parent;
  m_parent->setGraphicsEffect (p_blur);
  m_parent->setDisabled (true);

  /* attach shadow widget */
  m_shadow->setParent (a_parent);
  m_shadow->resize (a_parent->size());
  m_shadow->move (0,0);
  m_shadow->show();

  /* centering */
  setParent (a_parent->parentWidget());
  move (a_parent->width() / 2 - width() / 2,
        a_parent->height() / 2 - height() / 2);

  /* setup text */
  ui->lTitle->setText (a_title);
  ui->lDescription->setText (a_description);

  if (a_btnNo.isEmpty()){
    ui->btnNo->hide();
    ui->spacer->hide();
  }

  if (a_btnYes.isEmpty()){
    ui->btnYes->hide();
    ui->spacer->hide();
  }

  ui->btnYes->setText (a_btnYes);
  ui->btnNo->setText (a_btnNo);

  /* text size */
//  int th = ui->lDescription->fontMetrics().boundingRect(QRect(0,0,100,100), 0, a_description).height();
//  if (th > dafaultTextHeight)
//  {
//      // increase window size
//      setMinimumHeight (dafaultHeight + th - dafaultTextHeight);
//      setMaximumHeight (dafaultHeight + th - dafaultTextHeight);
//  }
//  else
//  {
//      // default size
//      setMinimumHeight (dafaultHeight);
//      setMaximumHeight (dafaultHeight);
//  }
  _adjustHeight (a_description);

  /* display */
  show();
}

void PopupDialog::_slotButtonPress()
{
  /* delete effect and enable parent wodget */
  m_parent->setGraphicsEffect (nullptr);
  m_parent->setEnabled (true);

  /* hide shadow overlay and this dialog */
  m_shadow->close();
  close();

  /* unattach shadow overlay and this dialog */
  m_shadow->setParent (nullptr);
  setParent (nullptr);
}

/*-----------------------------------------*/
