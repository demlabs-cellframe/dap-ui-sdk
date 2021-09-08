/* INCLUDES */
#include "documentpopup.h"
#include "ui_documentpopup.h"
#include "kelguicommon.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DocumentPopup::DocumentPopup (Type t, QWidget *parent) :
  QWidget (parent),
  ui (new Ui::DocumentPopup)
{
  ui->setupUi (this);

  switch (t)
    {
    case TermsOfUse: slotShowTermsOfUse(); break;
    case PrivacyPolicy: slotShowPrivacyPolicy(); break;
    }
}

DocumentPopup::~DocumentPopup()
{
  delete ui;
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void DocumentPopup::slotShowTermsOfUse()
{
  setObjectName ("Terms of use");
  ui->label->setText ("Terms of use");
  ui->textField->setHtml (Common::fromFile (":/gui/ui/data/terms.html"));
}

void DocumentPopup::slotShowPrivacyPolicy()
{
  setObjectName ("Privacy policy");
  ui->label->setText ("Privacy policy");
  ui->textField->setHtml (Common::fromFile (":/gui/ui/data/privacy.html"));
}

/*-----------------------------------------*/
