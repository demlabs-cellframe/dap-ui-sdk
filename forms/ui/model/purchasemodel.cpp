/* INCLUDES */
#include "purchasemodel.h"

#include "dapguibutton.h"

/* DEFS */

enum _LicenceType
{
  LT_MONTH,
  LT_YEAR,
};

struct _Licence
{
  double score, price;
  int length;
  _LicenceType type;
  QString description;
};

/* VARS */
static QList<_Licence> s_licences =
{
  {4.96,  0.00,  1,  LT_MONTH,  ""},
  {26.88, 4.48,  6,  LT_MONTH,  "$4.48 per month"},
  {35.88, 2.99,  1,  LT_YEAR,   "$2.99 per month"},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

PurchaseModel::PurchaseModel (QWidget *parent)
  : QScrollArea (parent),
    lay (nullptr)
{

}

PurchaseModel::~PurchaseModel()
{

}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void PurchaseModel::slotSetup()
{
  setupLayout();

  /* add every item */
  foreach (auto &item, s_licences)
    {
      /* new item */
      auto btn = new DapGuiButton;

      /* setup text */
      auto score = QString().sprintf ("$%.02lf", item.score);
      auto plan  =
        QString().sprintf (
          "%d-%s%s plan",
          item.length,
          item.type == LT_MONTH ? "month" : "year",
          item.length > 0 ? "s" : "");

      /* setup new widget */
      btn->setBtnStyle (DapGuiButton::LeftTopMainBottomSub);
      btn->setLeftText (score);
      btn->setMainText (plan);
      btn->setSubText (item.description);
      btn->setLeftCssClass ("font30 darkblue bold purchase-item-text-left");
      btn->setMainCssClass ("font18 darkblue normalbold purchase-item-text-main");
      btn->setSubCssClass ("font16 gray purchase-item-text-sub");
      btn->setCssStyle ("purchase-item");
      btn->setFrame (true);
      //btn->setMaximumSize (357, 96);


      /* add into list */
      lay->addWidget (btn);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
}

/********************************************
 * PRIVATE METHODS
 *******************************************/

void PurchaseModel::setupLayout()
{
  /* setup model */
  if (lay)
    return;

  /* if widget exists */
  if (widget())
    {
      /* if layout not present */
      if (!layout())
        {
          /* cerate and store */
          lay    = new QVBoxLayout (widget());
          widget()->setLayout (lay);
        }

      /* get existing layout */
      else
        lay = dynamic_cast<QVBoxLayout *> (widget()->layout());
    }
  /* no widget -> no model job */
  else
    return;
}

/*-----------------------------------------*/
