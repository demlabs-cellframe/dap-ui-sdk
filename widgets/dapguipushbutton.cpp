/* INCLUDES */
#include "dapguipushbutton.h"
#include "dapguicommon.h"

#include <QFile>
#include <QStylePainter>
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
  {DapGuiPushButton::Main,        {"://gfx/%1/btn_bg.png",         "://gfx/%1/btn_bg_hover_active.png",   "://gfx/%1/btn_bg_hover_active.png",   "cwpb_bigbutton"}},
  {DapGuiPushButton::Connection,  {"://gfx/%1/btn_connection.png", "://gfx/%1/btn_connection.png", "://gfx/%1/btn_connection_active.png", "cwpb_square"}},
  {DapGuiPushButton::Account,     {"://gfx/%1/btn_account.png",    "://gfx/%1/btn_account.png",    "://gfx/%1/btn_account_active.png",    "cwpb_square"}},
  {DapGuiPushButton::Settings,    {"://gfx/%1/btn_settings.png",   "://gfx/%1/btn_settings.png",   "://gfx/%1/btn_settings_active.png",   "cwpb_square"}},
  {DapGuiPushButton::Check,       {"://gfx/%1/check_btn_off.png",  "://gfx/%1/check_btn_on.png",          "://gfx/%1/check_btn_on.png",          "cwpb_checkbox"}},
  //{DapGuiPushButton::Radio,       {"://gfx/%1/radio_btn_off.png",  "://gfx/%1/radio_btn_on.png",          "://gfx/%1/radio_btn_on.png",         "cwpb_radio"}},
  {DapGuiPushButton::Switch,      {"://gfx/%1/switch_off.png",     "://gfx/%1/switch.png",                "://gfx/%1/switch.png",                "cwpb_switch"}},
  {DapGuiPushButton::Close,       {"://gfx/%1/ic_close.png",       "://gfx/%1/ic_close_hover.png",        "://gfx/%1/ic_close_hover.png",        "cwpb_close"}},
  {DapGuiPushButton::FilterClear, {"://gfx/%1/btn_filter_clear.png","://gfx/%1/btn_filter_clear_hover.png",        "://gfx/%1/ic_close_hover.png", "cwpb_close"}},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiPushButton::DapGuiPushButton (QWidget *parent)
  : QPushButton (parent)
  , m_style (Main)
  , m_opacityEffect (new QGraphicsOpacityEffect)
  , _styleHook (false)
{
  setFocusPolicy (Qt::NoFocus);
  setStyle (m_style);

  /* setup opacity */
  m_opacityEffect->setOpacity (0.99);
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



/****************************************//**
 * @property DapGuiPushButton::style
 * @brief button's built-in style.
 *
 * All built-in styles stored in
 * static **QMap s_presets** inside
 * **dapguipushbutton.cpp**
 *
 * @accessors %style(), %setStyle()
 * @see DapGuiPushButton::Style
 *******************************************/

DapGuiPushButton::Style DapGuiPushButton::style() const
{
  return m_style;
}

void DapGuiPushButton::setStyle (const Style &style)
{
  /* check hook */
  if(_styleHook)
    return;

  /* start hook */
  _styleHook  = true;

  m_style = style;

  /* setup style, if present */
  if (s_presets.contains (style))
    {
      auto item = s_presets.value (style);
      auto dir  = __kgsm.themeDir();

      item.filename = item.filename.arg (dir);
      item.hover    = item.hover.arg (dir);
      item.checked  = item.checked.arg (dir);

      setCustom (item.filename.mid (2));
      setCustomHover (item.hover.mid (2));
      setCustomPushed (item.checked.mid (2));
      setCustomCss (item.cssStyle);

      /* free hook */
      _styleHook  = false;

      return emit styleChanged();
    }

  updateStyle();
  emit styleChanged();

  /* free hook */
  _styleHook  = false;
}

void DapGuiPushButton::setEnabledCustom(bool value)
{
  this->setEnabled(value);
  setOpacity (value);
}

/****************************************//**
 * @property DapGuiPushButton::opacity
 * @brief button's opacity effect
 *
 * Makes button semi-transparent
 *
 * Available opacity:
 * - 1 on true
 * - 0.2 on false
 *
 * @accessors %opacity(), %setOpacity()
 *******************************************/

bool DapGuiPushButton::opacity() const
{
  return m_opacityEffect->opacity() > 0.95;
}

void DapGuiPushButton::setOpacity(bool value)
{
  m_opacityEffect->setOpacity (value ? 0.99 : 0.2);
}

/****************************************//**
 * @property DapGuiPushButton::custom
 * @brief custom default state picture url
 * @note available only when
 * DapGuiPushButton::Style is Custom
 * @accessors %custom(), %setCustom()
 *******************************************/

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

/****************************************//**
 * @property DapGuiPushButton::customHover
 * @brief custom hover state picture url
 * @note available only when
 * DapGuiPushButton::Style is Custom
 * @accessors %customHover(), %setCustomHover()
 *******************************************/

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

/****************************************//**
 * @property DapGuiPushButton::customPushed
 * @brief custom pushed state picture url
 * @note available only when
 * DapGuiPushButton::Style is Custom
 * @accessors %customPushed(), %setCustomPushed()
 *******************************************/

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

/****************************************//**
 * @property DapGuiPushButton::cssStyle
 * @brief custom css style
 * @accessors %customCss(), %setCustomCss()
 *******************************************/

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
  __kgsm.forcedButtonStyleUpdate();
}

void DapGuiPushButton::paintEvent(QPaintEvent *)
{
  QStylePainter p (this);
  QString pxFile;

  QStyleOptionButton option;
  initStyleOption (&option);
  QStyle::State state = option.state;

  if (state & QStyle::State_On)
    pxFile  = customPushed().path();
  else if (state & QStyle::State_MouseOver)
    pxFile  = customHover().path();
  else
    pxFile  = custom().path();

  QRect cr          = contentsRect();
  QSize scaledSize  = cr.size() * devicePixelRatioF();
  if (_cache.size != scaledSize ||  _cache.filename != pxFile)
  {
      _cache.filename   = pxFile;
      _cache.size       = scaledSize;
      _cache.pixmap = QPixmap(":/" + pxFile)
              .scaled(
                scaledSize,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation);
  }

  p.drawPixmap (cr, _cache.pixmap);
  p.drawItemText (rect(), Qt::AlignCenter, palette(), isEnabled(), text(), QPalette::ButtonText);
  //p.drawControl (QStyle::CE_PushButton, option);
}

/*-----------------------------------------*/

