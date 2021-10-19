/* INCLUDES */
#include "dapguipushbutton.h"
#include "dapguicommon.h"

#include <QFile>
#include <QStyleOption>
#include <QPainter>

#include <QMouseEvent>

#include <QVBoxLayout>
#include <QHBoxLayout>

/* DEFS */
struct _KgpbItem
{
  QString filename, hover, checked, cssStyle;
};

/* VARS */
static QMap<DapGuiPushButton::Style, _KgpbItem> s_presets =
{
  {DapGuiPushButton::Main,        {"://gfx/btn_bg.png",         "://gfx/btn_bg_hover_active.png",   "://gfx/btn_bg_hover_active.png",   "cwpb_bigbutton"}},
  {DapGuiPushButton::Connection,  {"://gfx/btn_connection.png", "://gfx/btn_connection_active.png", "://gfx/btn_connection_active.png", "cwpb_square"}},
  {DapGuiPushButton::Account,     {"://gfx/btn_account.png",    "://gfx/btn_account_active.png",    "://gfx/btn_account_active.png",    "cwpb_square"}},
  {DapGuiPushButton::Settings,    {"://gfx/btn_settings.png",   "://gfx/btn_settings_active.png",   "://gfx/btn_settings_active.png",   "cwpb_square"}},
  {DapGuiPushButton::Check,       {"://gfx/check_btn_off.png",  "://gfx/check_btn_on.png",          "://gfx/check_btn_on.png",          "cwpb_checkbox"}},
  //{DapGuiPushButton::Radio,       {"://gfx/radio_btn_off.png",  "://gfx/radio_btn_on.png",          "://gfx/radio_btn_on.png",         "cwpb_radio"}},
  {DapGuiPushButton::Switch,      {"://gfx/switch_off.png",     "://gfx/switch.png",                "://gfx/switch.png",                "cwpb_switch"}},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiPushButton::DapGuiPushButton (QWidget *parent)
  : QPushButton (parent)
  , m_style (Main)
  , m_opacityEffect (new QGraphicsOpacityEffect)
#ifdef ENABLEPURPLE
  , m_text ("Push Button")
  , m_label (new QLabel (m_text))
  , m_effect (new QGraphicsDropShadowEffect)
#endif // ENABLEPURPLE
{
  //setObjectName ("DapGuiPushButton");
  //setFlat(true);
  setFocusPolicy (Qt::NoFocus);

#ifdef ENABLEPURPLE
  auto hor = new QHBoxLayout;
  hor->addWidget (m_label.get());

  setAttribute (Qt::WA_Hover);
  m_label->setObjectName ("DapGuiPushButtonText");
  m_label->setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);

  setLayout (hor);

  m_effect->setBlurRadius (5);
  m_effect->setXOffset (5);
  m_effect->setYOffset (5);
  m_effect->setColor (QColor::fromRgba (qRgba (0x40, 0x14, 0x24, 0x24)));
  m_effect->setEnabled (true);

  setGraphicsEffect (m_effect);
#endif // ENABLEPURPLE

  //setStyleSheet (fromFile ("://styles/pushbutton.css"));
  //updateStyle();
  setStyle (m_style);

  /* setup opacity */
  m_opacityEffect->setOpacity (1);
  m_opacityEffect->setEnabled (true);
  setGraphicsEffect (m_opacityEffect);
  setAutoFillBackground (true);
  setEnabled(isEnabled()); // refresh opacity

  /* signals */
  connect (this, &QPushButton::clicked,
           this, &DapGuiPushButton::clicked);
}

DapGuiPushButton::~DapGuiPushButton()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

DapGuiPushButton::Style DapGuiPushButton::style() const
{
  return m_style;
}

void DapGuiPushButton::setStyle (const Style &style)
{
  m_style = style;

  /* setup style, if present */
  if (s_presets.contains (style))
    {
      auto item = s_presets.value (style);
      setCustom (item.filename.mid (2));
      setCustomHover (item.hover.mid (2));
      setCustomPushed (item.checked.mid (2));
      setCustomCss (item.cssStyle);
      return emit styleChanged();
    }

  updateStyle();
  emit styleChanged();
}

void DapGuiPushButton::setEnabledCustom(bool value)
{
  this->setEnabled(value);
  setOpacity (value);
}

void DapGuiPushButton::setOpacity(bool value)
{
  m_opacityEffect->setOpacity (value ? 1 : 0.2);
}

#ifdef ENABLEPURPLE
QString DapGuiPushButton::text() const
{
  return m_text;
}

void DapGuiPushButton::setText (const QString &text)
{
  m_text = text;
  m_label->setText (text);
  updateStyle();
  emit textChanged();
}
#endif // ENABLEPURPLE

QUrl DapGuiPushButton::custom() const
{
  return m_custom;
}

void DapGuiPushButton::setCustom (const QUrl &custom)
{
  m_custom = custom;
  updateStyle();
  emit customChanged();
}

QUrl DapGuiPushButton::customHover() const
{
  return m_customHover;
}

void DapGuiPushButton::setCustomHover (const QUrl &customHover)
{
  m_customHover = customHover;
  updateStyle();
  emit customHoverChanged();
}

QUrl DapGuiPushButton::customPushed() const
{
  return m_customPushed;
}

void DapGuiPushButton::setCustomPushed (const QUrl &customPushed)
{
  m_customPushed = customPushed;
  updateStyle();
  emit customPushedChanged();
}

QString DapGuiPushButton::customCss() const
{
  return m_customCss;
}

void DapGuiPushButton::setCustomCss (const QString &customCss)
{
  m_customCss = customCss;
  updateStyle();
}

bool DapGuiPushButton::customEnabled() const
{
  return m_style == Custom;
}

void DapGuiPushButton::updateStyle()
{
#ifdef ENABLEPURPLE
  if (m_style == Purple)
    {
      m_label->hide();
      return setStyleSheet (fromFile ("://styles/pushbutton.css"));
    }
#endif // ENABLEPURPLE

//  /* collect 3 files name */
//  QString names[4] =
//  {
//    tr (":/") + m_custom.path(),
//    tr (":/") + m_customHover.path(),
//    tr (":/") + m_customPushed.path(),
//    m_customCss,
//  };

//  /* get first file pixmap */
//  QPixmap img (names[0]);

//  /* setup style sheet based on pixmap size and files names */
//  QString style =
//    Common::fromFile ("://styles/pushbuttoncustom.css")
//    .arg (img.size().width())
//    .arg (img.size().height())
//    .arg (names[0], names[1], names[2]);

//  setStyleSheet (style);

  __kgsm.forcedButtonStyleUpdate();

#ifdef ENABLEPURPLE
  m_label->show();
#endif // ENABLEPURPLE
}

/********************************************
 * OVERRIDE
 *******************************************/

//void DapGuiPushButton::paintEvent (QPaintEvent *)
//{
//  QStyleOption opt;
//  opt.init (this);

//  QPainter p (this);
//  QWidget::style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
//}

//void DapGuiPushButton::mousePressEvent(QMouseEvent *event)
//{
//  if (event->button() == Qt::LeftButton)
//    emit s_clicked();
//}

//bool DapGuiPushButton::event(QEvent *e)
//{
//  switch(e->type())
//    {
//    case QEvent::HoverEnter:
//      {
//        m_effect->setEnabled (false);
//        return true;
//      }break;

//    case QEvent::HoverLeave:
//      {
//        m_effect->setEnabled (m_style == Purple);
//        return true;
//      }break;

//    default:break;
//    }

//  return QWidget::event(e);
//}

/*-----------------------------------------*/

