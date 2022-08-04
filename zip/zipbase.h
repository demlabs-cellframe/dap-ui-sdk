#ifndef ZIPBASE_H
#define ZIPBASE_H

#include <QtCore>
#include "zipdef.h"
#include "zlib.h"

namespace DapZip {

    struct ZipFileInfo
    {
        ZipFileInfo() noexcept
            : isDir(false), isFile(false), isSymLink(false), crc(0), size(0)
        {}

        bool isValid() const noexcept { return isDir || isFile || isSymLink; }

        QString filePath;
        uint isDir : 1;
        uint isFile : 1;
        uint isSymLink : 1;
        QFile::Permissions permissions;
        uint crc;
        qint64 size;
        QDateTime lastModified;
    };

    class ZipBase
    {
    public:
        ZipBase(QIODevice *device, bool ownDev)
            : device(device), ownDevice(ownDev), dirtyFileTree(true), start_of_directory(0)
        {
        }

        ~ZipBase()
        {
            if (ownDevice)
                delete device;
        }

        ZipFileInfo fillFileInfo(int index) const;

        QIODevice *device;
        bool ownDevice;
        bool dirtyFileTree;
        QVector<FileHeader> fileHeaders;
        QByteArray comment;
        uint start_of_directory;
    };

    uint readUInt(const uchar *data);
    ushort readUShort(const uchar *data);
    void writeUInt(uchar *data, uint i);
    void writeUShort(uchar *data, ushort i);
    void copyUInt(uchar *dest, const uchar *src);
    void copyUShort(uchar *dest, const uchar *src);
    void writeMSDosDate(uchar *dest, const QDateTime& dt);
    int inflate(Bytef *dest, ulong *destLen, const Bytef *source, ulong sourceLen);
    int deflate (Bytef *dest, ulong *destLen, const Bytef *source, ulong sourceLen);
    QFile::Permissions modeToPermissions(quint32 mode);
    quint32 permissionsToMode(QFile::Permissions perms);
    QDateTime readMSDosDate(const uchar *src);
    LocalFileHeader toLocalHeader(const CentralFileHeader &ch);

}

#endif // ZIPBASE_H
