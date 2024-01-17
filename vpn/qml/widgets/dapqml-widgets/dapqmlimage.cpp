/* INCLUDES */
#include "dapqmlimage.h"
#include "helper/scaling.h"
#include "dapqmlstyle.h"
#include "dapqml-model/dapqmlmodelroutingexceptions.h"

#include <QPainter>
#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QTimer>

/* DEFINES*/

#define WORKERS_NUMBER (4)

/// Process Image Operation Information

struct ImageProcessItem
{
  QString filename;
  QSize size;
  DapQmlImageItem *destination;
};

/// Process Image Worker's Thread Container

class ImageProcessorThread
{
  QThread *thread;
  DapQmlImageItemProcessWorker *worker;

public:
  ImageProcessorThread();
  ~ImageProcessorThread();

  void invoke();
};

/* VARIABLES */

static const char *s_imageProvider  = "DapQmlModelRoutingExceptionsImageProvider";
static QQueue<ImageProcessItem> s_queue;
static QMutex s_mutex;
static QList<ImageProcessorThread*> s_workers;
static int s_activeWorkers = 0;

/* FUNCTIONS */

static void queueImageOperation (const ImageProcessItem &a_item);
static void wokeUpThreads();

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlImageItem::DapQmlImageItem (QQuickItem *parent)
  : QQuickPaintedItem (parent)
{
  connect (this, &DapQmlImageItem::_sigRedraw,
           this, &DapQmlImageItem::slotRedraw,
           Qt::QueuedConnection);
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlImageItem::initWorkers()
{
  for (int i = 0; i < WORKERS_NUMBER; i++)
    s_workers << new ImageProcessorThread;
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

void DapQmlImageItem::_setupImage (DapImage &&a_image, const QSizeF &a_size)
{
  _cache.size   = a_size;
  _cache.name   = m_scaledPixmap;
  _cache.image  = std::move (a_image);
  emit _sigRedraw();
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlImageItem::slotRedraw()
{
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
      queueImageOperation(
        ImageProcessItem
        {
          m_scaledPixmap,
          size,
          this
        });
      return QTimer::singleShot (25, []{ wokeUpThreads(); });
    }

  /* draw */
  //a_painter->drawPixmap (content, QPixmap::fromImage (_cache.image));
  a_painter->drawImage (QRectF { content.topLeft(), content.size() }, _cache.image);
}

/*-----------------------------------------*/

void queueImageOperation (const ImageProcessItem &a_item)
{
  QMutexLocker l (&s_mutex);
  s_queue << a_item;
}

void wokeUpThreads()
{
  if (s_activeWorkers == WORKERS_NUMBER)
    return;

  for (auto worker : qAsConst (s_workers))
    worker->invoke();
}

/*-----------------------------------------*/

DapQmlImageItemProcessWorker::DapQmlImageItemProcessWorker()
{

}

void DapQmlImageItemProcessWorker::slotProcess()
{
  /* variables */
  ImageProcessItem item;
  DapImage result;

  //while (true)
  {
    /* get operation info */
    {
      QMutexLocker l (&s_mutex);
      if (s_queue.isEmpty())
      {
//        QTimer::singleShot (10, []
//          {
//            if (s_activeWorkers == 0)
//              DapQmlStyle::sRequestRedraw();
//          });
        return;
      }

      /* activate */
      item  = s_queue.dequeue();
      s_activeWorkers++;
    }

    /* fix name */
    QString filename  = item.filename;
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
        result =
          image.scaled(
            item.size,
            Qt::IgnoreAspectRatio,
            DapImage::SmoothTransformation);
    }
    else if (item.filename.contains (s_imageProvider))
      {
        /* calc filename */
        int index         = item.filename.indexOf (s_imageProvider);
        QString filename  = item.filename.mid (index + strlen (s_imageProvider) + 1);

        /* get pixmap */
        QImage image      = DapQmlModelRoutingExceptionsImageProvider::instance()->requestImage (
                              filename, nullptr, item.size);

        /* scale pixmap */
        result =
          DapImage (image).scaled(
            item.size,
            Qt::IgnoreAspectRatio,
            DapImage::SmoothTransformation);
      }

    /* set result */
    item.destination->_setupImage (std::move (result), item.size);

    /* deactivate */
    {
      QMutexLocker l (&s_mutex);
      s_activeWorkers--;
    }

    QMetaObject::invokeMethod (this, "slotProcess", Qt::QueuedConnection);
  }
}

ImageProcessorThread::ImageProcessorThread()
{
  worker  = new DapQmlImageItemProcessWorker;
}

ImageProcessorThread::~ImageProcessorThread()
{
  worker->deleteLater();
  thread->exit();
  thread->deleteLater();
}

void ImageProcessorThread::invoke()
{
  QMetaObject::invokeMethod (worker, "slotProcess", Qt::QueuedConnection);
}

/*-----------------------------------------*/
