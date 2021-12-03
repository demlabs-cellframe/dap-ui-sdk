/* INCLUDES */
#include "historymodel.h"
#include "DapDataLocal.h"
#include <QApplication>
#include <QClipboard>
#include <QScrollBar>

/* DEFS */
struct _HistoryRecord
{
  QString name;
};

/* VARS */
static QList<QString> s_history =
{
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
    {"GTSF-WWRM-KLMWQ-OWWM"},
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
  clearLayout(lay);
  m_list.clear();

  /* create new buttons */
  foreach (auto &item, DapDataLocal::instance()->getHistorySerialKeyData())
    {
      /* create item */
      DapGuiButton *btn = new DapGuiButton;
      m_list << btn;

      btn->setBtnStyle (DapGuiButton::IconMainSub);

      btn->setMainText (item);
      btn->setMainCssClass ("darkblue uppercase font16");

      btn->setSubText (" ");

      btn->setSeparator (true);
      btn->setIconCssClass ("history-icon ic_key-item");

      btn->setSubCssClass ("history-icon ic_copy");

      btn->setCursor(Qt::PointingHandCursor);

      btn->setCssStyle ("history-item");

      connect(btn, &DapGuiButton::clicked, [btn](){
          QApplication::clipboard()->setText(btn->mainText());
      });
      lay->addWidget (btn);
    }
  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
}

/********************************************
 * OVERRIDE
 *******************************************/

bool HistoryModel::eventFilter(QObject *o, QEvent *e)
{
  // This works because QScrollArea::setWidget installs an eventFilter on the widget
  if(o && o == widget() && e->type() == QEvent::Resize)
    setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());

  return QScrollArea::eventFilter(o, e);
}

/*-----------------------------------------*/
