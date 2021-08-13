/* INCLUDES */
#include "DapCmdParser.h"
#include <QFile>
#include <QDateTime>

/// TODO : unit test

/* VARS */
static QFile *file = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdParser::DapCmdParser (QTcpSocket *sockCtl, QObject *parent, bool dumpMessages)
  : QObject (parent),
    m_sockCtl (sockCtl),
    m_dumpMessages (dumpMessages)
{
  /* connect signal */
  if (m_sockCtl)
    connect (m_sockCtl, &QTcpSocket::readyRead,
             this, &DapCmdParser::onCtlReadReady);
  if (m_dumpMessages)
    {
      auto dt = QDateTime::currentDateTime()
                .toString ("HH:mm:ss_dd-MM-yyyy");
      file = new QFile (QString("/opt/kelvpn/dump/") + dt + ".rdmp");
      file->open (QIODevice::WriteOnly);
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
      if (m_dumpMessages && file)
        {
          if (file->isOpen())
            {
              auto dt = QDateTime::currentDateTime()
                        .toString ("HH:mm:ss_dd-MM-yyyy");

              file->write (
                QString ("@@%1@@\n").arg (dt)
                .toLocal8Bit());
              file->write (readBuffer);
              file->flush();
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

/*-----------------------------------------*/
