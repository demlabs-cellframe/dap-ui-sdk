/* INCLUDES */
#include "bugreportsmodel.h"

/* DEFS */
enum _ReportType
{
  RT_INVALID,

  RT_INDEV,
  RT_PROC,
  RT_RESOLVED,
};

struct _ReportRecord
{
  QString name;
  _ReportType type;
};

/* VARS */
static QList<_ReportRecord> s_history =
{
  {"Report #1264", RT_INDEV},
  {"Report #1270", RT_PROC},
  {"Report #3335", RT_RESOLVED},
  {"Report #1288", RT_RESOLVED},
};

static QMap<_ReportType, const char*> s_repNameMap =
{
  {RT_INVALID,  "Invalid item"},
  {RT_INDEV,    "In development"},
  {RT_PROC,     "In processing"},
  {RT_RESOLVED, "Resolved"},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

BugReportsModel::BugReportsModel(QWidget *parent)
  : ModelBase (parent)
{

}

BugReportsModel::~BugReportsModel()
{

}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void BugReportsModel::slotSetup()
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
      auto btn = new DapGuiButton;
      m_list << btn;

      btn->setBtnStyle (DapGuiButton::IconMainSub);

      btn->setMainText (text);
      btn->setMainCssClass ("darkblue lato font16");

      btn->setSubText (s_repNameMap.value (item.type));

      btn->setSeparator (true);
      btn->setIconCssClass ("bugrep-icon ic_information_bug-report");

      btn->setSubCssClass ("gray font12 lato");

      btn->setCssStyle ("bugrep-item");
      lay->addWidget (btn);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
}

/*-----------------------------------------*/
