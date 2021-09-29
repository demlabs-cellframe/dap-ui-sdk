/* INCLUDES */
#include "kelguiswitch.h"
#include "ui_kelguiswitch.h"
//#include "ui/helper/auxiliary/AppStyleSheetHandler.h"

#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>

#include <QPropertyAnimation>

/* DEFS */
#define TOGGLE_ON_POS (270 - 174)

/* VARS */
KelGuiSwitch::P2pCallback KelGuiSwitch::m_p2p = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiSwitch::KelGuiSwitch (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::KelGuiSwitch)
  , m_checked (false)
  , m_toggleOnPos (TOGGLE_ON_POS)
{
  /* setup style */
  ui->setupUi (this);
  __kgsm.forcedStyleUpdate();

  /* setup animation */
  m_animToggle = new QPropertyAnimation (ui->switch_toggle, "pos");
  m_animToggle->setDuration (100);
  _setAnimByState();

  m_animGroup.addAnimation (m_animToggle);

  /* signals */
  connect (&m_animGroup, &QParallelAnimationGroup::finished,
           this, &KelGuiSwitch::_setAnimByState);
}

/********************************************
 * METHODS
 *******************************************/

bool KelGuiSwitch::isChecked() const
{
  return m_checked;
}

void KelGuiSwitch::setChecked(bool a_checked)
{
  /* update position based on DPI */
//  float dpi     = UiScaling::getNativDPI();
//  m_toggleOnPos = UiScaling::pointsToPixels (TOGGLE_ON_POS, dpi);
  if(m_p2p)
    m_toggleOnPos = m_p2p (TOGGLE_ON_POS);

  /* animation */
  _setAnimByState();
  m_animGroup.start();

  /* store */
  m_checked = a_checked;

  /* change style */
  ui->switch_bg->setCssStyle (
        m_checked ? "swbg sw-on" : "swbg sw-off");
  ui->switch_toggle->setCssStyle (
        m_checked ? "sw-toggle sw-toggle-on" : "sw-toggle sw-toggle-off");

  /* move toggle */
  //ui->switch_toggle->move (m_checked ? m_toggleOnPos : 0, 0);

  /* signal */
  emit stateChanged (m_checked);
}

void KelGuiSwitch::toggle()
{
  setChecked (!isChecked());
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiSwitch::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    {
      emit clicked();
      toggle();
      _debugInfoClipboard();
    }
}

/********************************************
 * METHODS
 *******************************************/

void KelGuiSwitch::setP2p(P2pCallback newP2p)
{
  m_p2p = newP2p;
}

/********************************************
 *
 *******************************************/

void KelGuiSwitch::_setAnimByState()
{
  m_animToggle->setStartValue (ui->switch_toggle->pos());
  m_animToggle->setEndValue (
      m_checked
      ? QPoint (0, ui->switch_toggle->y())
      : QPoint (m_toggleOnPos, ui->switch_toggle->y())
    );
}

void KelGuiSwitch::_debugInfoClipboard()
{
#ifdef QT_DEBUG
  if(qApp->keyboardModifiers() & Qt::AltModifier)
    {
      QJsonObject jobj;
      jobj["cssStyle"]      = QJsonObject{
        {"name", __kgsm.cssStyle()},
        {"text", styleSheet()},
        {"switch_bg_css", ui->switch_bg->cssStyle()},
        {"switch_bg_qss", ui->switch_bg->styleSheet()},
        {"switch_toggle_css", ui->switch_toggle->cssStyle()},
        {"switch_toggle_qss", ui->switch_toggle->styleSheet()},
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

/*-----------------------------------------*/
