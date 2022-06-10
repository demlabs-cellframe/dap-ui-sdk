/* INCLUDES */
#include "dapqmlimage.h"
#include <QPainter>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlImage::DapQmlImage (QQuickItem *parent)
  : QQuickPaintedItem (parent)
{

}

/********************************************
 * METHODS
 *******************************************/

QString DapQmlImage::scaledPixmap() const
{
  return m_scaledPixmap;
}

void DapQmlImage::setScaledPixmap (const QString &a_scaledPixmap)
{
  if (m_scaledPixmap == a_scaledPixmap)
    return;

  m_scaledPixmap  = a_scaledPixmap;
  _cache.size     = QSize();

  emit sigScaledPixmapChanged();
  update();
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapQmlImage::paint (QPainter *a_painter)
{
  /* check, if no pixmap provided */
  if (m_scaledPixmap.isEmpty())
    return;

  /* get size */
  auto content  = contentsBoundingRect().toAlignedRect();
  auto size     = content.size();

  /* check, if cache has needed size image */
  if (_cache.size != size)
    {
      /* fix name */
      QString filename  = m_scaledPixmap;
      if (filename.startsWith("qrc:/"))
        filename.replace (0, 5, "://");

      /* read file */
      QFile file (filename);
      if (file.open (QIODevice::ReadOnly))
        {
          /* read contents */
          QPixmap image;
          image.loadFromData (file.readAll());

          /* scale pixmap */
          _cache.size   = size;
          _cache.pixmap = image
              .scaled (size,
                       Qt::IgnoreAspectRatio,
                       Qt::SmoothTransformation);
        }
    }

  /* draw */
  a_painter->drawPixmap (content, _cache.pixmap);
}

/*-----------------------------------------*/
