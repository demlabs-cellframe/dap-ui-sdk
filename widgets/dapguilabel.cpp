/* INCLUDES */
#include "dapguilabel.h"
#include "dapguicommon.h"

#include <QMutex>
#include <QMap>
#include <QMouseEvent>
#include <QStylePainter>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiLabel::DapGuiLabel (QWidget *parent)
  : QLabel (parent)
  , _cssHook (false)
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
  m_scaledPixmap = scaledPixmap;
  setPixmap (scaledPixmap);
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapGuiLabel::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();
}

void DapGuiLabel::paintEvent(QPaintEvent *e)
{
  if (m_scaledPixmap.isEmpty())
    return QLabel::paintEvent (e);

  QStylePainter p (this);
  QStyleOptionFrame option;
  initStyleOption (&option);
  option.state  |= QStyle::State_Enabled;

  QRect cr          = contentsRect();
  QSize scaledSize  = cr.size() * devicePixelRatioF();
  QPixmap px  = pixmap()->scaled(
        scaledSize,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);

  p.drawPixmap (cr, px);
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
