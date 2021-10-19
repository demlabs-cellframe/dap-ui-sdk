/* INCLUDES */
#include "dapguicommon.h"
#include <QFile>

/* NAMESPACE */
namespace Common
{

QString fromFile (const QString &filename)
{
  QFile file (filename);
  file.open (QIODevice::ReadOnly);
  return file.readAll();
}

};

/*-----------------------------------------*/
