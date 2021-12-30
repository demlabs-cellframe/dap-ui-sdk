/* INCLUDES */
#include "bugreportsmodel.h"
#include <QScrollBar>

/* DEFS */
enum _ReportType
{
  RT_INVALID,

  RT_INDEV,
  RT_PROC,
  RT_RESOLVED,
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
  clearLayout(lay);
  m_list.clear();

  /* create new buttons */
  foreach (auto &item, *DapDataLocal::instance()->bugReportHistory()->getBugReportsList())
    {
      /* create item */
      auto btn = new DapGuiButton;
      m_list << btn;

      btn->setBtnStyle (DapGuiButton::IconMainSub);

      btn->setMainText ("Report #" + item.number);
      btn->setMainCssClass ("darkblue lato font16");

      btn->setSubText (item.status);

      btn->setSeparator (true);
      btn->setIconCssClass ("bugrep-icon ic_information_bug-report");

      btn->setSubCssClass ("gray font12 lato");

      btn->setCssStyle ("bugrep-item");
      lay->addWidget (btn);
    }
  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
}

bool BugReportsModel::eventFilter(QObject *o, QEvent *e)
{
    // This works because QScrollArea::setWidget installs an eventFilter on the widget
    if(o && o == widget() && e->type() == QEvent::Resize)
        setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());

    return QScrollArea::eventFilter(o, e);
}

/*-----------------------------------------*/
