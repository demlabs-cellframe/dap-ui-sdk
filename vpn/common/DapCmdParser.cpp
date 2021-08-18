/* INCLUDES */
#include "DapCmdParser.h"
#include <QFile>
#include <QDateTime>
#include <QXmlStreamReader>
#include <QTimer>
#include <QQueue>
#include <QJsonDocument>
#include <QJsonObject>

/* DEFS */

/**
 * @brief controller for communacation simulation
 *
 * This class will parse test file (which is actualy a dump),
 * fill up queue and start timer with lambda connected to it.
 *
 * When timer get timeout it will invoke perform() method.
 *
 * Inside perform() method, message will be taken from queue
 * and sended via signal cmdReady()
 *
 * When queue become empty, class will destroy itself.
 *
 * @note there is several tests can be passed into this class.
 * To do it, just open it as "file" Qfile instance and then
 * create class instance
 * @code
 *  file  = new QFile ("://service_common_messages.xml");
 *  file->open(QIODevice::ReadOnly);
 *  sim   = new CommunicationSimulation(this);
 * @endcode
 */
class CommunicationSimulation
{
  // defs
  struct Item
  {
    int id;
    int interval;
    QByteArray content;
  };

  // vars
  QTimer m_timer;
  QQueue<Item> m_queue;
  DapCmdParser *m_parser;
  int m_last;
public:
  CommunicationSimulation (DapCmdParser *parser);
  void perform();
};

/* VARS */
static QFile *file = nullptr;
static CommunicationSimulation *sim = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdParser::DapCmdParser (QTcpSocket *sockCtl, QObject *parent, TestMode dumpMessages)
  : QObject (parent),
    m_sockCtl (sockCtl),
    m_testMode (dumpMessages)
{
  /* connect signal */
  if (m_sockCtl)
    connect (m_sockCtl, &QTcpSocket::readyRead,
             this, &DapCmdParser::onCtlReadReady);

  /* init tests by mode */
  switch(m_testMode)
    {

    /* create messages dump */
    case TM_DUMP:
      {
        auto dt = QDateTime::currentDateTime()
                  .toString ("HH:mm:ss_dd-MM-yyyy");
        file = new QFile (QString ("/opt/kelvpn/dump/") + dt + ".rdmp");
        file->open (QIODevice::WriteOnly);
      } break;

    /* test common messages from service */
    case TM_COMMON:
      {
        file  = new QFile ("://service_common_messages.xml");
        file->open(QIODevice::ReadOnly);
        sim   = new CommunicationSimulation(this);
      } break;

    /* test service messages while service is connecting */
    case TM_SERVICECONNECTINPROCESS:
      {
        file  = new QFile ("://service_while_connecting.xml");
        file->open(QIODevice::ReadOnly);
        sim   = new CommunicationSimulation(this);
      } break;

    case TM_NONE: break;

    }
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void DapCmdParser::onCtlReadReady()
{
  /* vars */
  int nInd;

  /* read message */
  auto readBytes  = m_sockCtl->readAll();

  /* read in cycle */
lb_read_str:

  /* check for packet end symbol presence */
  if ((nInd = readBytes.indexOf ("}\n\n")) == -1)
    {
      /* warn */
      qDebug() << "No CR symbol";
      readBuffer += readBytes;

      /* check buffer overflow */
      if (readBuffer.size() > MAX_BUFFER_SIZE)
        {
          qWarning() << "Buffer is full. All data is dropped";
          return;
        }
    }
  else
    {
      /* move idx to second \n */
      nInd        += 2;

      /* get message and send as signal argument */
      readBuffer  += readBytes.left (nInd);
      emit cmdReady (readBuffer);

      /* dump message, if required */
      if (m_testMode == TM_DUMP && file)
        {
          if (file->isOpen())
            {
              static quint32 counter = 0;
              static QDateTime last;
              auto dt = QDateTime::currentDateTime();
              //.toString ("HH:mm:ss_dd-MM-yyyy");

              if (!last.isValid())
                last = dt;

              int seconds = last.secsTo (dt);
              last = dt;

              file->write (
                //QString ("<item id=\"%2\" date=\"%1\">\n")
                QString ("@{\"id\": \"%2\",\"seconds\": \"\%1\", \"timestamp\": \"%3\"}\n")
                .arg (seconds).arg (counter).arg (dt.toString ("HH:mm:ss.zzz_dd-MM-yyyy"))
                .toLocal8Bit());
              file->write (readBuffer);
              file->flush();

              counter++;
            }
        }

      /* cut out beginning */
      readBytes   = readBytes.mid (nInd + 1);

      /* clear buffer */
      readBuffer.clear();

      /* loop, if another message exists */
      if (readBytes.length() > 0)
        {
          goto lb_read_str;
        }
    }
}

/********************************************
 * CommunicationSimulation
 *******************************************/

CommunicationSimulation::CommunicationSimulation(DapCmdParser *parser)
  : m_parser (parser)
{
  /* get all data */
  auto data   = file->readAll();

  /* split by '@' */
  auto array  = data.split ('@');

  /* cycle thru every item */
  for (auto &item : array)
    {
      /* get first line */
      auto len = item.indexOf ('\n');
      if (len == -1)
        continue; // skip if issue

      /* parse objects */
      QJsonObject jobj =
          QJsonDocument::fromJson (item.mid (0, len)).object();

      Item value
      {
        jobj.value ("id").toString().toInt(),
        jobj.value ("seconds").toString().toInt(),
        item.mid (len)
      };
      m_queue.append (
            value
            );
    }

  /* free */
  delete file;
  file = nullptr;

  /* check */
  if(m_queue.isEmpty())
    return;

  /* create timer */
  int time  = 3;//m_queue.head().interval - m_last;
  m_timer.setSingleShot(true);
  m_timer.setInterval(time*1000);
  m_last    = 0;//time;

  qDebug() << __PRETTY_FUNCTION__ << " starting simulation";

  /* signals */
  QObject::connect(&m_timer, &QTimer::timeout, []()
    {
      sim->perform();
    });

  m_timer.start();
}

void CommunicationSimulation::perform()
{
  /* vars */
  Item item;

  /* finish when done */
  if(m_queue.isEmpty())
    goto _finish_this;

  /* get item */
  item = m_queue.dequeue();

  /* send message */
  emit m_parser->cmdReady(item.content);

  /* finish when done */
  if(m_queue.isEmpty())
    {
_finish_this:
      auto sim  = ::sim;
      ::sim     = nullptr;
      delete sim;
      qDebug() << __PRETTY_FUNCTION__ << " simulation end";
      return;
    }

  /* setup new timer */
  int time  = m_queue.head().interval - m_last;
  m_timer.setInterval( (time<5) ? time*1000 : 5*1000 );
  m_last    = m_queue.head().interval;
  m_timer.start();
}

/*-----------------------------------------*/
