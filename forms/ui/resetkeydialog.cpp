/* INCLUDES */
#include "resetkeydialog.h"
#include "ui_resetkeydialog.h"

#include <QGraphicsBlurEffect>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ResetKeyDialog::ResetKeyDialog (QWidget *parent) :
  QWidget (parent),
  ui (new Ui::ResetKeyDialog),
  m_parent (nullptr),
  m_shadow (new QWidget)
{
  ui->setupUi (this);

  connect (ui->btnYes, &KelGuiLabel::clicked,
           this, &ResetKeyDialog::sigYes,
           Qt::QueuedConnection);
  connect (ui->btnNo, &KelGuiLabel::clicked,
           this, &ResetKeyDialog::sigNo,
           Qt::QueuedConnection);

  connect (ui->btnYes, &KelGuiLabel::clicked,
           this, &ResetKeyDialog::slotButtonPress,
           Qt::QueuedConnection);
  connect (ui->btnNo, &KelGuiLabel::clicked,
           this, &ResetKeyDialog::slotButtonPress,
           Qt::QueuedConnection);

  m_shadow->setStyleSheet (
    "background-color: rgba(0,0,0,0.5);"
  );
}

ResetKeyDialog::~ResetKeyDialog()
{
  delete ui;
}

/********************************************
 * SLOTS
 *******************************************/

void ResetKeyDialog::slotShow (QWidget *parent)
{
  QGraphicsBlurEffect *p_blur = new QGraphicsBlurEffect;
  p_blur->setBlurRadius (5);
  p_blur->setBlurHints (QGraphicsBlurEffect::PerformanceHint);

  m_parent  = parent;
  m_parent->setGraphicsEffect (p_blur);
  m_parent->setDisabled (true);

  m_shadow->setParent (parent);
  m_shadow->resize (parent->size());
  m_shadow->move (0,0);
  m_shadow->show();

  setParent (parent->parentWidget());
  move (parent->width() / 2 - width() / 2,
        parent->height() / 2 - height() / 2);
  show();
}

void ResetKeyDialog::slotButtonPress()
{
  m_parent->setGraphicsEffect (nullptr);
  m_parent->setEnabled (true);

  m_shadow->close();
  close();

  m_shadow->setParent (nullptr);
  setParent (nullptr);
}

/*-----------------------------------------*/
