/* INCLUDES */
#include "dapqmlimage.h"
#include "helper/scaling.h"
#include "dapqmlstyle.h"
#include "dapqml-model/dapqmlmodelroutingexceptions.h"
#include "dapqmlimage/imagescalingthreadpool.h"

#include <QPainter>
#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <QPointer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlImageItem::DapQmlImageItem (QQuickItem *parent)
  : QQuickPaintedItem (parent)
{
  connect (this, &DapQmlImageItem::_sigRedraw,
           this, &DapQmlImageItem::_slotRedraw,
           Qt::QueuedConnection);
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlImageItem::initWorkers ()
{
  ImageScalingThreadPool::instance();
}

QString DapQmlImageItem::scaledPixmap() const
{
  return m_scaledPixmap;
}

void DapQmlImageItem::setScaledPixmap (const QString &a_scaledPixmap)
{
  if (m_scaledPixmap == a_scaledPixmap)
    return;

  m_scaledPixmap  = a_scaledPixmap;
  _cache.size     = QSize();
  _cache.name     = QString();

  emit sigScaledPixmapChanged();
  update();
}

//void DapQmlImageItem::_setupImage (DapImage &&a_image, const QSizeF &a_size)
//{
//  _cache.size   = a_size;
//  _cache.name   = m_scaledPixmap;
//  _cache.image  = std::move (a_image);
//  emit _sigRedraw();
//}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlImageItem::_slotRedraw()
{
  update();
}

void DapQmlImageItem::_slotScalingFinished()
{
  disconnect (_cache.conn);
  ImageScalingThreadPool::instance()->requestResult (_cache.name, _cache.size.toSize(), _cache.image);
  update();
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapQmlImageItem::paint (QPainter *a_painter)
{
  /* check, if no pixmap provided */
  if (m_scaledPixmap.isEmpty())
    return;

  /* get size */
  auto content  = contentsBoundingRect().toAlignedRect();
  auto size     = content.size();

  /* calc actual size */
  //auto actualDpi  = Scaling::getPhysicalDPI();
  auto scaleMul = Scaling::getDevicePixelRatio();
  size  = QSize
  {
    static_cast<int> (size.width() * scaleMul),
    static_cast<int> (size.height() * scaleMul)
  };

  /* check, if cache has needed size image */
  if (_cache.size != size
      || _cache.name != m_scaledPixmap)
    {
      _cache.size   = size;
      _cache.name   = m_scaledPixmap;

#ifndef ANDROID
      if (ImageScalingThreadPool::isActive())
      {
        _cache.conn   = connect (ImageScalingThreadPool::instance(), &ImageScalingThreadPool::sigFinished,
                                 this, &DapQmlImageItem::_slotScalingFinished);
        ImageScalingThreadPool::instance()->queueScaling (m_scaledPixmap, size);
        return;
      }
      else
        ImageScalingThreadPool::performScaling (m_scaledPixmap, size, _cache.image);
#else // ANDROID
      ImageScalingThreadPool::performScaling (m_scaledPixmap, size, _cache.image);
#endif // ANDROID
    }

  /* draw */
  //a_painter->drawPixmap (content, QPixmap::fromImage (_cache.image));
  a_painter->drawImage (QRectF { content.topLeft(), content.size() }, _cache.image);
}

/*-----------------------------------------*/
