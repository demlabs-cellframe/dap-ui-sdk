/* INCLUDES */

#include "imagescalingthreadpool.h"
#include "dapqml-model/dapqmlmodelroutingexceptions.h"

#include <QQueue>
#include <QHash>
#include <QThread>
#include <QTimer>
#include <QFile>

/* DEFINES */

// amount of worker threads
#define WORKERS_NUMBER (4)
#define QUEUE_TIMEOUT_TIME (50)
#define CLEAR_TIMEOUT_TIME (3000)


// operation info
struct QueuedScalingOperation
{
  QString filename;
  QSize size;
};


// operation result
struct ScalingResult
{
  QString filename;
  QSize size;
  DapImage result;
};


// operation signature
// contains size and filename
// example: "128x128|qrc://image.png"
class ScalingOperationSignature
{
  /* variables */
  QSize m_size;
  QString m_filename;

  /* constructors */
public:
  ScalingOperationSignature();
  ScalingOperationSignature (const ScalingOperationSignature &a_source);
  ScalingOperationSignature (ScalingOperationSignature &&a_source);
  ScalingOperationSignature (const QString &a_source);
  ScalingOperationSignature (const QSize &a_size, const QString &a_filename);

  /* methods */
  const QSize &size() const         { return m_size; }
  const QString &filename() const   { return m_filename; }
  QString asString() const;
};


// container for thread and its worker
struct WorkerThread
{
  /* variables */
  ImageScalingWorker *_worker;
  QThread *_thread;

  /* construct/destruct */
public:
  WorkerThread();
  ~WorkerThread();

  /* methods */
public:
  ImageScalingWorker *worker() const { return _worker; }
};

/* VARIABLES */

static const char *s_imageProvider  = "DapQmlModelRoutingExceptionsImageProvider";
static QHash<QString, QueuedScalingOperation> s_queue;
static QHash<QString, ScalingResult> s_resultMap;
static QList<WorkerThread*> s_workers;
static QTimer *s_queueTimeout = nullptr;
static QTimer *s_clearTimeout = nullptr;
static bool s_isActive  = false;

/* FUNCTIONS */

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ImageScalingThreadPool::ImageScalingThreadPool()
  : QObject()
{
  s_queueTimeout  = new QTimer;
  s_clearTimeout  = new QTimer;
  s_queueTimeout->setSingleShot (true);
  s_clearTimeout->setSingleShot (true);
  s_queueTimeout->setInterval (QUEUE_TIMEOUT_TIME);
  s_clearTimeout->setInterval (CLEAR_TIMEOUT_TIME);

  for (int i = 0; i < WORKERS_NUMBER; i++)
    s_workers << new WorkerThread;

  /* queue */
  connect (s_queueTimeout, &QTimer::timeout, this, [this]
    {
      /* check if nothing to do */
      if (s_queue.isEmpty())
      {
        /* start clear timer */
        emit _sigStartClearTimer(); // s_clearTimeout->start();

        /* send notification */
        emit sigFinished();
      }

      /* begin workers iteration */
      auto worker = s_workers.begin();

      /* cycle thru every item from queue */
      for (auto &item : s_queue)
      {
        /* send operation data */
        QMetaObject::invokeMethod(
            (*worker)->worker(),
            "slotProcess",
            Qt::QueuedConnection,
            Q_ARG(QString, item.filename),
            Q_ARG(QSize, item.size)
          );

        /* advance workers */
        worker++;

        if (worker == s_workers.end())
          worker = s_workers.begin();
      }
    });

  /* clear */
  connect (s_clearTimeout, &QTimer::timeout, []
    {
      //s_queue.clear();
    });

  /* signals */
  connect (this, &ImageScalingThreadPool::_sigStartQueueTimer,
           s_queueTimeout, QOverload<>::of (&QTimer::start));
  connect (this, &ImageScalingThreadPool::_sigStartClearTimer,
           s_clearTimeout, QOverload<>::of (&QTimer::start));
}

ImageScalingThreadPool::~ImageScalingThreadPool()
{
  delete s_queueTimeout;
  s_queueTimeout  = nullptr;

  qDeleteAll (s_workers);
  s_workers.clear();
}

/********************************************
 * METHODS
 *******************************************/

ImageScalingThreadPool *ImageScalingThreadPool::instance()
{
  static ImageScalingThreadPool i;
  return &i;
}

void ImageScalingThreadPool::queueScaling (const QString &a_filename, const QSize &a_newSize)
{
  if (!s_isActive)
    return;

  QString signature     = ScalingOperationSignature (a_newSize, a_filename).asString();
  if (!s_resultMap.contains (signature))
    s_queue.insert (signature, QueuedScalingOperation { a_filename, a_newSize });

  emit _sigStartQueueTimer(); // s_queueTimeout->start();
}

void ImageScalingThreadPool::requestResult (const QString &a_filename, const QSize &a_newSize, DapImage &a_dest)
{
  if (!s_isActive)
    return;

  QString signature     = ScalingOperationSignature (a_newSize, a_filename).asString();
  ScalingResult result  = s_resultMap.value (signature);

  if (result.size == a_newSize
      && result.filename  == a_filename)
    a_dest  = std::move (result.result);
}

void ImageScalingThreadPool::performScaling(
  const QString &a_filename,
  const QSize &a_newSize,
  DapImage &a_dest)
{
  /* fix name */
  QString filename  = a_filename;
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
      a_dest =
        image.scaled(
          a_newSize,
          Qt::IgnoreAspectRatio,
          DapImage::SmoothTransformation);
  }
  else if (a_filename.contains (s_imageProvider))
    {
      /* calc filename */
      int index         = a_filename.indexOf (s_imageProvider);
      QString filename  = a_filename.mid (index + strlen (s_imageProvider) + 1);

      /* get pixmap */
      QImage image      = DapQmlModelRoutingExceptionsImageProvider::instance()->requestImage (
                            filename, nullptr, a_newSize);

      /* scale pixmap */
      a_dest =
        DapImage (image).scaled(
          a_newSize,
          Qt::IgnoreAspectRatio,
          DapImage::SmoothTransformation);
    }
}

bool ImageScalingThreadPool::isActive()
{
  return s_isActive;
}

void ImageScalingThreadPool::setActive (bool a_active)
{
  s_isActive  = a_active;
}

/********************************************
 * SLOTS
 *******************************************/

void ImageScalingThreadPool::slotScalingResult (const QString &a_filename, const QSize &a_newSize, const DapImage &a_result)
{
  /* store result */
  s_resultMap.insert(
    ScalingOperationSignature (a_newSize, a_filename).asString(),
    ScalingResult { a_filename, a_newSize, a_result }
  );

  /* check if finished */
  if (s_resultMap.size() >= s_queue.size())
  {
    /* start clear timer */
    emit _sigStartClearTimer(); // s_clearTimeout->start();

    /* send notification */
    emit sigFinished();
  }
}

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ImageScalingWorker::ImageScalingWorker()
{

}

/********************************************
 * SLOTS
 *******************************************/

void ImageScalingWorker::slotProcess (const QString &a_filename, const QSize &a_newSize)
{
  /* variables */
  DapImage result;

  /* scale */
  ImageScalingThreadPool::performScaling (a_filename, a_newSize, result);

  /* send result data */
  QMetaObject::invokeMethod(
      ImageScalingThreadPool::instance(),
      "slotScalingResult",
      Qt::QueuedConnection,
      Q_ARG(QString, a_filename),
      Q_ARG(QSize, a_newSize),
      Q_ARG(DapImage, result)
    );
}

/*-----------------------------------------*/

ScalingOperationSignature::ScalingOperationSignature()
{

}

QString ScalingOperationSignature::asString() const
{
  return QString ("%1x%2|%3")
    .arg (m_size.width())
    .arg (m_size.height())
    .arg (m_filename);
}

ScalingOperationSignature::ScalingOperationSignature (const ScalingOperationSignature &a_source)
  : m_size (a_source.m_size)
  , m_filename (a_source.m_filename)
{
}

ScalingOperationSignature::ScalingOperationSignature (ScalingOperationSignature &&a_source)
{
  if (this != &a_source)
  {
    m_size      = std::move (a_source.m_size);
    m_filename  = std::move (a_source.m_filename);
  }
}

ScalingOperationSignature::ScalingOperationSignature (const QString &a_source)
{
  int separator = a_source.indexOf ('|');
  if (separator == -1)
    return;

  QString sizeSource  = a_source.mid (0, separator);
  QStringList split   = sizeSource.split ('x');
  if (split.size() != 2)
    return;

  m_size      = QSize { split.at (0).toInt(), split.at (1).toInt() };
  m_filename  = a_source.mid (separator + 1);
}

ScalingOperationSignature::ScalingOperationSignature (const QSize &a_size, const QString &a_filename)
  : m_size (a_size)
  , m_filename (a_filename)
{

}

/*-----------------------------------------*/

WorkerThread::WorkerThread()
{
  _thread  = new QThread;
  _worker  = new ImageScalingWorker;

  _worker->moveToThread (_thread);
  _thread->setObjectName ("ImageScalingWorker" + QString::number (s_workers.size() + 1));
  _thread->start();
}

WorkerThread::~WorkerThread()
{
  _worker->deleteLater();
  _thread->exit();
  _thread->deleteLater();
}

/*-----------------------------------------*/
