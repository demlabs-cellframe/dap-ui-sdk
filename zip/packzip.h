#ifndef PACKZIP_H
#define PACKZIP_H
#include "zipbase.h"

/*!
 Disabling copy and move func.
 The compiler gives an error message.
*/
#define DISABLE_COPY_MOVE(T) \
    T(const T&) = delete;\
    T &operator=(const T&) = delete; \
    T(T&&) = delete; \
    T &operator=(T&&) = delete; \

/*! Compress example:

#include "packzip.h"
DapZip::PackZip zip("archive.zip");
zip.addFile("firstfiletoarchive.txt");
zip.addFile("secondfiletoarchive.txt");
zip.close();

*/


namespace DapZip
{

    // AlwaysCompress   A file that is added is compressed.
    // NeverCompress    A file that is added will be stored without changes.
    // AutoCompress     A file that is added will be compressed only if that will give a smaller file.
    enum CompressionPolicy {
        AlwaysCompress,
        NeverCompress,
        AutoCompress
    };

    // NoError          No error occurred.
    // FileReadError    An error occurred when reading from the file.
    // FileOpenError    The file could not be opened.
    // FilePermissionsError The file could not be accessed.
    // FileError        Another file error occurred.
    enum Status {
        NoError,
        FileWriteError,
        FileOpenError,
        FilePermissionsError,
        FileError
    };

    class PackZipPrivate : public ZipBase
    {
    public:
        PackZipPrivate(QIODevice *device, bool ownDev)
            : ZipBase(device, ownDev),
            status(DapZip::NoError),
            permissions(QFile::ReadOwner | QFile::WriteOwner),
            compressionPolicy(DapZip::AlwaysCompress)
        {
        }

        DapZip::Status status;
        QFile::Permissions permissions;
        DapZip::CompressionPolicy compressionPolicy;

        enum EntryType { Directory, File, Symlink };

        void addEntry(EntryType type, const QString &fileName, const QByteArray &contents);
    };

    class PackZip
    {
    public:
        explicit PackZip(const QString &fileName, QIODevice::OpenMode mode = (QIODevice::WriteOnly | QIODevice::Truncate) );
        explicit PackZip(QIODevice *device);
        ~PackZip();

        QIODevice* device() const;

        bool isWritable() const;
        bool exists() const;

        Status status() const;

        void setCompressionPolicy(CompressionPolicy policy);
        CompressionPolicy compressionPolicy() const;

        void setCreationPermissions(QFile::Permissions permissions);
        QFile::Permissions creationPermissions() const;
        void addFile(const QString &fileName);
        void addFile(const QString &fileName, const QByteArray &data);
        void addFile(const QString &fileName, QIODevice *device);
        void addDirectory(const QString &dirName);
        void addSymLink(const QString &fileName, const QString &destination);

        void close();
    private:
        PackZipPrivate *d;
//        Disabling copy and move func.
//        The compiler gives an error message.
        DISABLE_COPY_MOVE(PackZip)
    };

    bool compressFiles(const QString archive, const QStringList fileList);
}

#endif // PACKZIP_H