#ifndef ZIPBASE_H
#define ZIPBASE_H

#include <QtCore>
#include <QStringRef>
#include "zipdef.h"
#include "zlib.h"

namespace DapZip {

    // NoError          No error occurred.
    // FileReadError    An error occurred when reading from the file.
    // FileWriteError   An error occurred when writing to the file.
    // FileOpenError    The file could not be opened.
    // FilePermissionsError The file could not be accessed.
    // FileError        Another file error occurred.
    enum Status {
        NoError,
        FileReadError,
        FileWriteError,
        FileOpenError,
        FilePermissionsError,
        FileError
    };

    // FileInfo        Represents one entry in the zip table of contents.
    // filePath        The full filepath inside the archive.
    // isDir           A boolean type indicating if the entry is a directory.
    // isFile          A boolean type, if it is one this entry is a file.
    // isSymLink       A boolean type, if it is one this entry is symbolic link.
    // permissions     A list of flags for the permissions of this entry.
    // crc             The calculated checksum as a crc type.
    // size            The total size of the unpacked content.
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

/*!
 Disabling copy and move func.
 The compiler gives an error message.
*/
#define DISABLE_COPY_MOVE(T) \
    T(const T&) = delete;\
    T &operator=(const T&) = delete; \
    T(T&&) = delete; \
    T &operator=(T&&) = delete; \

#endif // ZIPBASE_H
