/* INCLUDES */
#include "bugreportsmodel.h"
#include <QScrollBar>
#include <QEvent>

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

#ifndef TestApp
  /* create new buttons */
  foreach (auto &item, *DapDataLocal::instance()->bugReportHistory()->getBugReportsList())
    {
      /* create item */
      auto btn = new DapGuiButton;
      m_list << btn;

      btn->setBtnStyle (DapGuiButton::IconMainSubIcon);

      btn->setMainText (tr("Report #") + item.number);
      btn->setMainCssClass ("darkblue lato font16");

      btn->setSubText (item.status);
      btn->setSubCssClass ("gray font12 lato");
      btn->setSubAlignment (Qt::AlignLeft | Qt::AlignVCenter);

      btn->setSeparator (true);
      btn->setIconCssClass ("bugrep-icon ic_information_bug-report");
      btn->setIconRightCssClass ("bugrep-icon-right ic_trash");

      btn->setCssStyle ("bugrep-item");
      lay->addWidget (btn);

      connect (btn, &DapGuiButton::rightIconClicked,
               this, &BugReportsModel::slotTrashClicked);
    }
#endif // TestApp
  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
}

void BugReportsModel::slotTrashClicked(const QString &a_number)
{
  qDebug() << __FUNCTION__;
  DapDataLocal::instance()->removeItemFromHistory(TEXT_BUGREPORT_HISTORY, a_number);
}

bool BugReportsModel::eventFilter(QObject *o, QEvent *e)
{
    // This works because QScrollArea::setWidget installs an eventFilter on the widget
    if(o && o == widget() && e->type() == QEvent::Resize)
        setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());

    return QScrollArea::eventFilter(o, e);
}

/*-----------------------------------------*/
