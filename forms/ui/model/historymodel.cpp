/* INCLUDES */
#include "historymodel.h"

/* DEFS */
struct _HistoryRecord
{
  QString name;
};

/* VARS */
static QList<_HistoryRecord> s_history =
{
  {"GTSF-WWRM-KLMWQ-OWWM"},
  {"GTSF-WWRM-KLMWQ-OWWM"},
  {"GTSF-WWRM-KLMWQ-OWWM"},
  {"GTSF-WWRM-KLMWQ-OWWM"},
  {"GTSF-WWRM-KLMWQ-OWWM"},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

HistoryModel::HistoryModel (QWidget *parent)
  : ModelBase (parent)
{

}

HistoryModel::~HistoryModel()
{

}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void HistoryModel::slotSetup()
{
  setupLayout();

  /* delete old buttons */
  foreach (auto *oldItem, m_list)
    {
      lay->removeWidget (oldItem);
      delete oldItem;
    }
  m_list.clear();

  /* create new buttons */
  foreach (auto &item, s_history)
    {
      /* get data */
      QString text = item.name;

      /* create item */
      auto btn = new KelGuiButton;
      m_list << btn;

      btn->setBtnStyle (KelGuiButton::IconMainSub);

      btn->setMainText (text);
      btn->setMainCssClass ("darkblue uppercase font16");

      btn->setSubText (" ");

      btn->setSeparator (true);
      btn->setIconCssClass ("history-icon ic_key-item");

      btn->setSubCssClass ("history-icon ic_copy");

      btn->setCssStyle ("history-item");
      lay->addWidget (btn);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
}

/*-----------------------------------------*/
