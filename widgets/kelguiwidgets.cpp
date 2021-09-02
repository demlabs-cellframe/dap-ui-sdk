#include <QFile>

QString fromFile (const QString &filename)
{
  QFile file (filename);
  file.open (QIODevice::ReadOnly);
  return file.readAll();
}
