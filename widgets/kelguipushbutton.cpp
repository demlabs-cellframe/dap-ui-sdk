/* INCLUDES */
#include "kelguipushbutton.h"
#include "kelguicommon.h"

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
static QMap<KelGuiPushButton::Style, _KgpbItem> s_presets =
{
  {KelGuiPushButton::Main,        {"://gfx/btn_bg.png",         "://gfx/btn_bg_hover_active.png",   "://gfx/btn_bg_hover_active.png",   "cwpb_bigbutton"}},
  {KelGuiPushButton::Connection,  {"://gfx/btn_connection.png", "://gfx/btn_connection_active.png", "://gfx/btn_connection_active.png", "cwpb_square"}},
  {KelGuiPushButton::Account,     {"://gfx/btn_account.png",    "://gfx/btn_account_active.png",    "://gfx/btn_account_active.png",    "cwpb_square"}},
  {KelGuiPushButton::Settings,    {"://gfx/btn_settings.png",   "://gfx/btn_settings_active.png",   "://gfx/btn_settings_active.png",   "cwpb_square"}},
  {KelGuiPushButton::Check,       {"://gfx/check_btn_off.png",  "://gfx/check_btn_on.png",          "://gfx/check_btn_on.png",          "cwpb_checkbox"}},
  //{KelGuiPushButton::Radio,       {"://gfx/radio_btn_off.png",  "://gfx/radio_btn_on.png",          "://gfx/radio_btn_on.png",         "cwpb_radio"}},
  {KelGuiPushButton::Switch,      {"://gfx/switch_off.png",     "://gfx/switch.png",                "://gfx/switch.png",                "cwpb_switch"}},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiPushButton::KelGuiPushButton (QWidget *parent)
  : QPushButton (parent)
  , m_style (Main)
#ifdef ENABLEPURPLE
  , m_text ("Push Button")
  , m_label (new QLabel (m_text))
  , m_effect (new QGraphicsDropShadowEffect)
#endif // ENABLEPURPLE
{
  //setObjectName ("KelGuiPushButton");
  //setFlat(true);

#ifdef ENABLEPURPLE
  auto hor = new QHBoxLayout;
  hor->addWidget (m_label.get());

  setAttribute (Qt::WA_Hover);
  m_label->setObjectName ("KelGuiPushButtonText");
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

  /* signals */
  connect (this, &QPushButton::clicked,
           this, &KelGuiPushButton::clicked);
}

KelGuiPushButton::~KelGuiPushButton()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

KelGuiPushButton::Style KelGuiPushButton::style() const
{
  return m_style;
}

void KelGuiPushButton::setStyle (const Style &style)
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

#ifdef ENABLEPURPLE
QString KelGuiPushButton::text() const
{
  return m_text;
}

void KelGuiPushButton::setText (const QString &text)
{
  m_text = text;
  m_label->setText (text);
  updateStyle();
  emit textChanged();
}
#endif // ENABLEPURPLE

QUrl KelGuiPushButton::custom() const
{
  return m_custom;
}

void KelGuiPushButton::setCustom (const QUrl &custom)
{
  m_custom = custom;
  updateStyle();
  emit customChanged();
}

QUrl KelGuiPushButton::customHover() const
{
  return m_customHover;
}

void KelGuiPushButton::setCustomHover (const QUrl &customHover)
{
  m_customHover = customHover;
  updateStyle();
  emit customHoverChanged();
}

QUrl KelGuiPushButton::customPushed() const
{
  return m_customPushed;
}

void KelGuiPushButton::setCustomPushed (const QUrl &customPushed)
{
  m_customPushed = customPushed;
  updateStyle();
  emit customPushedChanged();
}

QString KelGuiPushButton::customCss() const
{
  return m_customCss;
}

void KelGuiPushButton::setCustomCss (const QString &customCss)
{
  m_customCss = customCss;
  updateStyle();
}

bool KelGuiPushButton::customEnabled() const
{
  return m_style == Custom;
}

void KelGuiPushButton::updateStyle()
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

//void KelGuiPushButton::paintEvent (QPaintEvent *)
//{
//  QStyleOption opt;
//  opt.init (this);

//  QPainter p (this);
//  QWidget::style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
//}

//void KelGuiPushButton::mousePressEvent(QMouseEvent *event)
//{
//  if (event->button() == Qt::LeftButton)
//    emit s_clicked();
//}

//bool KelGuiPushButton::event(QEvent *e)
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

