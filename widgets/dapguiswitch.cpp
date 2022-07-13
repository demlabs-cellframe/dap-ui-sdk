/* INCLUDES */
#include "dapguiswitch.h"
#include "ui_dapguiswitch.h"
//#include "ui/helper/auxiliary/AppStyleSheetHandler.h"

#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>

#include <QPropertyAnimation>
#include <QTimer>
#include <QDebug>

/* DEFS */
#define TOGGLE_ON_POS (270 - 174)
#define BG_POS (12)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiSwitch::DapGuiSwitch (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::DapGuiSwitch)
  , m_checked (false)
  , m_toggleOnPos (TOGGLE_ON_POS)
  , m_animGroup (new QParallelAnimationGroup)
  , m_opacityEffect (new QGraphicsOpacityEffect)
{
  /* setup style */
  ui->setupUi (this);
  QMetaObject::invokeMethod(&__kgsm, &DapGuiSwitchStyleManager::forcedStyleUpdate, Qt::QueuedConnection);
  m_bgPos[0] = BG_POS;
  m_bgPos[1] = BG_POS * 2;

  /* setup opacity */
  setGraphicsEffect (m_opacityEffect);

  /* setup animation */
  m_animToggle = new QPropertyAnimation (ui->switch_toggle, "pos");
  m_animToggle->setDuration (100);
  _setAnimByState();

  m_animGroup->addAnimation (m_animToggle);

  /* signals */
  connect (ui->switch_bg, &DapGuiLabel::clicked,
           this, &DapGuiSwitch::_slotClicked);
  connect (ui->switch_toggle, &DapGuiLabel::clicked,
           this, &DapGuiSwitch::_slotClicked);

  connect (&__kgsm, &DapGuiSwitchStyleManager::styleUpdated,
           this, &DapGuiSwitch::_moveItems);
  connect (m_animGroup, &QParallelAnimationGroup::finished,
           this, &DapGuiSwitch::_setAnimByState);

  /* move items to proper places */
  _moveItems();
}

DapGuiSwitch::~DapGuiSwitch()
{
  m_animGroup->clear();
  m_animGroup->deleteLater();
}

/********************************************
 * METHODS
 *******************************************/




/****************************************//**
 * @property DapGuiSwitch::checked
 * @brief switch state
 * @accessors %checked(), %setChecked(), %toggle()
 *******************************************/

bool DapGuiSwitch::checked() const
{
  return m_checked;
}

void DapGuiSwitch::setChecked(bool a_checked)
{
  /* skip */
  if (m_checked == a_checked)
    return;

  /* update position based on DPI */
  {
    auto ww       = width();
    auto tw       = ui->switch_toggle->width();
    m_toggleOnPos = ww - tw + 2;
  }

  /* animation */
  _setAnimByState();
  m_animGroup->start();

  /* store */
  m_checked = a_checked;

  /* change style */
  ui->switch_bg->setCssStyle (
        m_checked ? "swbg sw-on" : "swbg sw-off");
  ui->switch_toggle->setCssStyle (
        m_checked ? "sw-toggle sw-toggle-on" : "sw-toggle sw-toggle-off");

  /* move toggle */
  //ui->switch_toggle->move (m_checked ? m_toggleOnPos : 0, 0);

  m_checked ? m_opacityEffect->setOpacity (0.4)
            : m_opacityEffect->setOpacity (0.7);

  /* signal */
  emit stateChanged (m_checked);
  emit toggled (m_checked);
}

void DapGuiSwitch::toggle()
{
  setChecked (!checked());
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapGuiSwitch::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    _slotClicked();
}

void DapGuiSwitch::paintEvent(QPaintEvent *)
{
  ui->switch_bg->setEnabled (true);
  ui->switch_toggle->setEnabled (true);

  QStyleOption opt;
  opt.init (this);
  opt.state |= QStyle::State_Enabled;

  auto state  = opt.state;
  qDebug() << state;

  QPainter p (this);
  style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

/********************************************
 *
 *******************************************/

void DapGuiSwitch::_setAnimByState()
{
  m_animToggle->setStartValue (ui->switch_toggle->pos());
  m_animToggle->setEndValue (
      m_checked
      ? QPoint (0, ui->switch_toggle->y())
      : QPoint (m_toggleOnPos, ui->switch_toggle->y())
        );
}

void DapGuiSwitch::_moveItems()
{
  static bool hook = false;

  if (!hook)
    {
      hook = true;
      QMetaObject::invokeMethod(this, &DapGuiSwitch::_moveItems, Qt::QueuedConnection);
      return;
    }
  else
    hook = false;

  {
    auto ww       = width();
    auto hh       = height();
    auto tw       = ui->switch_toggle->width();
    auto bw       = ui->switch_bg->width();
    auto bh       = ui->switch_bg->height();
    m_toggleOnPos = ww - tw + 2;
    m_bgPos[0]    = (ww - bw) / 2;
    m_bgPos[1]    = (hh - bh) / 2;
  }

  ui->switch_toggle->move (m_checked ? m_toggleOnPos : 0, 0);
  ui->switch_bg->move (m_bgPos[0], m_bgPos[1]);
}

void DapGuiSwitch::_slotClicked()
{
  emit clicked();
  toggle();
  _debugInfoClipboard();
}

void DapGuiSwitch::_debugInfoClipboard()
{
#ifdef QT_DEBUG
  if(qApp->keyboardModifiers() & Qt::AltModifier)
    {
      QJsonObject jobj;
      jobj["switch_bg_css"]     = ui->switch_bg->cssStyle();
      jobj["switch_bg_qss"]     = ui->switch_bg->styleSheet();
      jobj["switch_toggle_css"] = ui->switch_toggle->cssStyle();
      jobj["switch_toggle_qss"] = ui->switch_toggle->styleSheet();
      jobj["cssStyle"]          = QJsonObject{
        {"name", __kgsm.cssStyle()},
        {"text", styleSheet()},
      };
      jobj["rect"] = QJsonObject{
        {"x",pos().x()},
        {"y",pos().y()},
        {"width",size().width()},
        {"heigh",size().height()},
      };
      auto dump = QJsonDocument (jobj).toJson();
      qApp->clipboard()->setText (dump);
    }
#endif // QT_DEBUG
}

void DapGuiSwitch::setEnabled(bool enable)
{
    QWidget::setEnabled(enable);
    m_opacityEffect->setEnabled (!enable);
}

/*-----------------------------------------*/
