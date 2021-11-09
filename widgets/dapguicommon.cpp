/* INCLUDES */
#include "dapguicommon.h"
#include <QFile>

/* NAMESPACE */
namespace Common
{

QString fromFile (const QString &filename)
{
  QFile file (filename);
  QString ret;
  if (file.open (QIODevice::ReadOnly)) {
    ret = file.readAll();
    file.close();
  }
  return ret;
}

};

/*-----------------------------------------*/
