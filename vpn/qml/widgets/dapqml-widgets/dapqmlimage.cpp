/* INCLUDES */
#include "dapqmlimage.h"
#include "helper/scaling.h"
#include "dapqml-model/dapqmlmodelroutingexceptions.h"

#include <QPainter>
#include <QFile>

/* VARS */
static const char *s_imageProvider  = "DapQmlModelRoutingExceptionsImageProvider";

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlImageItem::DapQmlImageItem (QQuickItem *parent)
  : QQuickPaintedItem (parent)
{

}

/********************************************
 * METHODS
 *******************************************/

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
  if (_cache.size != size || _cache.name != m_scaledPixmap)
    {
      /* fix name */
      QString filename  = m_scaledPixmap;
      if (filename.startsWith ("qrc:/"))
        filename.replace (0, 5, "://");

      /* read file */
      QFile file (filename);
      if (file.open (QIODevice::ReadOnly))
        {
          /* read contents */
          DapImage image;
          image.loadFromData (file.readAll());

          /* scale pixmap */
          _cache.name   = m_scaledPixmap;
          _cache.size   = size;
          _cache.image  = image.scaled (
                            size,
                            Qt::IgnoreAspectRatio,
                            DapImage::SmoothTransformation
                          );
//          _cache.image = image;
      }
      else if (m_scaledPixmap.contains (s_imageProvider))
        {
          /* calc filename */
          int index         = m_scaledPixmap.indexOf (s_imageProvider);
          QString filename  = m_scaledPixmap.mid (index + strlen (s_imageProvider) + 1);

          /* get pixmap */
          QImage image      = DapQmlModelRoutingExceptionsImageProvider::instance()->requestImage (
                                filename, nullptr, size);

          /* scale pixmap */
          _cache.name   = m_scaledPixmap;
          _cache.size   = size;
          _cache.image  = DapImage (image).scaled (
                            size,
                            Qt::IgnoreAspectRatio,
                            DapImage::SmoothTransformation);
        }
    }

  /* draw */
  //a_painter->drawPixmap (content, QPixmap::fromImage (_cache.image));
  a_painter->drawImage (QRectF { content.topLeft(), content.size() }, _cache.image);
}

//void DapQmlImageItem::paint (QPainter *a_painter)
//{
//  /* check, if no pixmap provided */
//  if (m_scaledPixmap.isEmpty())
//    return;

////#ifdef ANDROID
////  /* setup render options */
////  a_painter->setRenderHint (QPainter::Antialiasing, true);
////  a_painter->setRenderHint (QPainter::SmoothPixmapTransform, true);
////#endif // ANDROID

//  /* get size */
//  auto content  = contentsBoundingRect().toAlignedRect();
//  auto size     = content.size();

//  /* calc actual size */
//  //auto actualDpi  = Scaling::getPhysicalDPI();
//  auto scaleMul = Scaling::getDevicePixelRatio();
//  size  = QSize
//  {
//    static_cast<int> (size.width() * scaleMul),
//    static_cast<int> (size.height() * scaleMul),
//    //static_cast<int> (Scaling::pointsToPixels (size.width(), actualDpi)),
//    //static_cast<int> (Scaling::pointsToPixels (size.height(), actualDpi))
//  };

//  /* check, if cache has needed size image */
//  if (_cache.size != size || _cache.name != m_scaledPixmap)
//    {
////#ifdef ANDROID
////      /* check if already loaded */
////      if (!_cache.pixmap.isNull()
////          && _cache.name == m_scaledPixmap)
////        return a_painter->drawPixmap (content, _cache.pixmap);
////#endif // ANDROID

////      qDebug() << __PRETTY_FUNCTION__ << "size:" << size << content.size();// << "dpi:" << actualDpi;

//      /* fix name */
//      QString filename  = m_scaledPixmap;
//      if (filename.startsWith ("qrc:/"))
//        filename.replace (0, 5, "://");

//      /* read file */
//      QFile file (filename);
//      if (file.open (QIODevice::ReadOnly))
//        {
//          /* read contents */
//          QPixmap image;
//          image.loadFromData (file.readAll());

//          /* scale pixmap */
//          _cache.name   = m_scaledPixmap;
//          _cache.size   = size;
//          _cache.pixmap = image.scaled (
//                            size,
//                            Qt::IgnoreAspectRatio,
//                            Qt::SmoothTransformation);

////          QImage scaledResult (size, QImage::Format_ARGB32);
////          scaledResult.fill (Qt::transparent);

////          QPainter painter (&scaledResult);
////          painter.setRenderHint (QPainter::HighQualityAntialiasing);
////          painter.drawPixmap (QRect (0, 0, size.width(), size.height()), image);
////          painter.end();

////          _cache.pixmap = QPixmap::fromImage (scaledResult);
//        }
//      else if (m_scaledPixmap.contains (s_imageProvider))
//        {
//          /* calc filename */
//          int index         = m_scaledPixmap.indexOf (s_imageProvider);
//          QString filename  = m_scaledPixmap.mid (index + strlen (s_imageProvider) + 1);

//          /* get pixmap */
//          auto image        = DapQmlModelRoutingExceptionsImageProvider::instance()->requestPixmap (
//                                filename, nullptr, size);

//          /* scale pixmap */
//          _cache.name   = m_scaledPixmap;
//          _cache.size   = size;
//          _cache.pixmap = image.scaled (
//                            size,
//                            Qt::IgnoreAspectRatio,
//                            Qt::SmoothTransformation);

////            QImage scaledResult (size, QImage::Format_ARGB32);
////            scaledResult.fill (Qt::transparent);

////            QPainter painter (&scaledResult);
////            painter.setRenderHint (QPainter::HighQualityAntialiasing);
////            painter.drawPixmap (QRect (0, 0, size.width(), size.height()), image);
////            painter.end();

////            _cache.pixmap = QPixmap::fromImage (scaledResult);
//        }
//    }

//  /* draw */
//  a_painter->drawPixmap (content, _cache.pixmap);
//}

/*-----------------------------------------*/
