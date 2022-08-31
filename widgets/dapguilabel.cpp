/* INCLUDES */
#include "dapguilabel.h"
#include "dapguicommon.h"

#include <QMutex>
#include <QMap>
#include <QMouseEvent>
#include <QStylePainter>
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiLabel::DapGuiLabel (QWidget *parent)
  : QLabel (parent)
  , _cssHook (false)
  , _hovered (false)
  , _pressed (false)
{
  connect (&__kgsm, &DapGuiStyleManager::cssStyleChanged,
           this, &DapGuiLabel::_slotCssStyleUpdate);
  setScaledContents (true);
}

/********************************************
 * OVERRIDE
 *******************************************/

QString DapGuiLabel::scaledPixmap() const
{
  return m_scaledPixmap;
}

void DapGuiLabel::setScaledPixmap (const QString &scaledPixmap)
{
  if (m_scaledPixmap == scaledPixmap)
    return;

  m_scaledPixmap = scaledPixmap;
  setPixmap (scaledPixmap);
  _cache.size = QSize();

  emit scaledPixmapChanged();
}

QString DapGuiLabel::scaledPixmapEx() const
{
  return m_scaledPixmapEx.idle
      + '|' + m_scaledPixmapEx.hovered
      + '|' + m_scaledPixmapEx.pressed;
}

void DapGuiLabel::setScaledPixmapEx(const QString &scaledPixmapEx)
{
  auto list = scaledPixmapEx.split('|');

  if (list.size() != 3)
    {
      qDebug() << __PRETTY_FUNCTION__ << "not enough arguments! must be 3 arguments separated by '|' symbol!";
      return;
    }

  m_scaledPixmapEx.idle     = list[0];
  m_scaledPixmapEx.hovered  = list[1];
  m_scaledPixmapEx.pressed  = list[2];

  emit setScaledPixmapExChanged();
}

void DapGuiLabel::_calculateNewPixmapState()
{
  if (m_scaledPixmapEx.idle.isEmpty()
      || m_scaledPixmapEx.hovered.isEmpty()
      || m_scaledPixmapEx.pressed.isEmpty())
    return;

  if (_pressed)
    return setScaledPixmap (m_scaledPixmapEx.pressed);

  if (_hovered)
    return setScaledPixmap (m_scaledPixmapEx.hovered);

  setScaledPixmap (m_scaledPixmapEx.idle);
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapGuiLabel::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();

  _pressed  = true;
  update(); /// scedule a paint event
}

void DapGuiLabel::mouseReleaseEvent(QMouseEvent *)
{
  _pressed  = false;
  update(); /// scedule a paint event
}

void DapGuiLabel::enterEvent(QEvent *)
{
  _hovered  = true;
  update(); /// scedule a paint event
}

void DapGuiLabel::leaveEvent(QEvent *)
{
  _hovered  = false;
  update(); /// scedule a paint event
}

void DapGuiLabel::paintEvent(QPaintEvent *e)
{
  _calculateNewPixmapState();

  if (m_scaledPixmap.isEmpty())
    return QLabel::paintEvent (e);

  QStylePainter p (this);
  QStyleOptionFrame option;
  initStyleOption (&option);
  option.state  |= QStyle::State_Enabled;

  QRect cr          = contentsRect();
  QSize scaledSize  = cr.size() * devicePixelRatioF();
  if (_cache.size != scaledSize)
  {
      _cache.size   = scaledSize;
      _cache.pixmap = pixmap()->scaled(
            scaledSize,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation);
  }

  p.drawPixmap (cr, _cache.pixmap);
  p.drawItemText (rect(), Qt::AlignCenter, palette(), isEnabled(), text(), QPalette::ButtonText);
}

/********************************************
 * SLOTS
 *******************************************/

void DapGuiLabel::_slotCssStyleUpdate()
{
  if(_cssHook)
    {
      _cssHook = false;
      return;
    }

  _cssHook = true;

  /* transparent attach */
  auto css = __kgsm.cssStyle();
  if (!css.contains ("noborder") && !css.contains ("nobackground"))
    __kgsm.setCssStyle (__kgsm.cssStyle() + " noborder nobackground");
}

/*-----------------------------------------*/
