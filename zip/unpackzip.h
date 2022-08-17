#ifndef UNPACKZIP_H
#define UNPACKZIP_H
#include "zipbase.h"

/*! Extract example1:

#include "unpackzip.h"
DapZip::UnpackZip zip("archive.zip");
zip.extractAll("archive");
zip.close();

*/

/*! Extract example2:

#include "unpackzip.h"
DapZip::fileDecompression("archive.zip", "archive");
*/


namespace DapZip
{
    class UnpackZip;

    class UnpackZipPrivate : public ZipBase
    {
    public:
        UnpackZipPrivate(QIODevice *device, bool ownDev)
            : ZipBase(device, ownDev), status(Status::NoError)
        {
        }

        void scanFiles();

        Status status;
    };


    class UnpackZip
    {
    public:
        explicit UnpackZip(const QString &fileName, QIODevice::OpenMode mode = QIODevice::ReadOnly );
            explicit UnpackZip(QIODevice *device);
        ~UnpackZip();

        QIODevice* device() const;

        bool isReadable() const;
        bool exists() const;

        QVector<ZipFileInfo> fileInfoList() const;
        int count() const;

        ZipFileInfo entryInfoAt(int index) const;
        QByteArray fileData(const QString &fileName) const;
        bool extractAll(const QString &destinationDir) const;

        Status status() const;

        void close();

    private:
        UnpackZipPrivate *d;
        DISABLE_COPY_MOVE(UnpackZip)
    };

    bool fileDecompression(const QString archive, const QString &destinationDir);
}

#endif // UNPACKZIP_H
