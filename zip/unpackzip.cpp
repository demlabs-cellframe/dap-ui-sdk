#include "unpackzip.h"

using namespace DapZip;

//////////////////////////////  Reader

/*!
    \class UnpackZip::FileInfo
    \internal
    Represents one entry in the zip table of contents.
*/

/*!
    \variable FileInfo::filePath
    The full filepath inside the archive.
*/

/*!
    \variable FileInfo::isDir
    A boolean type indicating if the entry is a directory.
*/

/*!
    \variable FileInfo::isFile
    A boolean type, if it is one this entry is a file.
*/

/*!
    \variable FileInfo::isSymLink
    A boolean type, if it is one this entry is symbolic link.
*/

/*!
    \variable FileInfo::permissions
    A list of flags for the permissions of this entry.
*/

/*!
    \variable FileInfo::crc
    The calculated checksum as a crc type.
*/

/*!
    \variable FileInfo::size
    The total size of the unpacked content.
*/

/*!
    \class UnpackZip
    \internal
    \since 4.5

    \brief the UnpackZip class provides a way to inspect the contents of a zip
    archive and extract individual files from it.

    UnpackZip can be used to read a zip archive either from a file or from any
    device. An in-memory QBuffer for instance.  The reader can be used to read
    which files are in the archive using fileInfoList() and entryInfoAt() but
    also to extract individual files using fileData() or even to extract all
    files in the archive using extractAll()
*/

/*!
    Create a new zip archive that operates on the \a fileName.  The file will be
    opened with the \a mode.
*/
UnpackZip::UnpackZip(const QString &archive, QIODevice::OpenMode mode)
{
    QScopedPointer<QFile> f(new QFile(archive));
    const bool result = f->open(mode);
    Status status;
    const QFileDevice::FileError error = f->error();
    if (result && error == QFile::NoError) {
        status = NoError;
    } else {
        if (error == QFile::ReadError)
            status = FileReadError;
        else if (error == QFile::OpenError)
            status = FileOpenError;
        else if (error == QFile::PermissionsError)
            status = FilePermissionsError;
        else
            status = FileError;
    }

    d = new UnpackZipPrivate(f.data(), /*ownDevice=*/true);
    f.take();
    d->status = status;
}

/*!
    Create a new zip archive that operates on the archive found in \a device.
    You have to open the device previous to calling the constructor and only a
    device that is readable will be scanned for zip filecontent.
 */
UnpackZip::UnpackZip(QIODevice *device)
    : d(new UnpackZipPrivate(device, /*ownDevice=*/false))
{
    Q_ASSERT(device);
}

/*!
    Desctructor
*/
UnpackZip::~UnpackZip()
{
    close();
    delete d;
}

/*!
    Returns device used for reading zip archive.
*/
QIODevice* UnpackZip::device() const
{
    return d->device;
}

/*!
    Returns \c true if the user can read the file; otherwise returns \c false.
*/
bool UnpackZip::isReadable() const
{
    return d->device->isReadable();
}

/*!
    Returns \c true if the file exists; otherwise returns \c false.
*/
bool UnpackZip::exists() const
{
    QFile *f = qobject_cast<QFile*> (d->device);
    if (f == nullptr)
        return true;
    return f->exists();
}

/*!
    Returns the list of files the archive contains.
*/
QVector<ZipFileInfo> UnpackZip::fileInfoList() const
{
    d->scanFiles();
    QVector<ZipFileInfo> files;
    const int numFileHeaders = d->fileHeaders.size();
    files.reserve(numFileHeaders);
    for (int i = 0; i < numFileHeaders; ++i)
        files.append(d->fillFileInfo(i));
    return files;

}

/*!
    Return the number of items in the zip archive.
*/
int UnpackZip::count() const
{
    d->scanFiles();
    return d->fileHeaders.count();
}

/*!
    Returns a FileInfo of an entry in the zipfile.
    The \a index is the index into the directory listing of the zipfile.
    Returns an invalid FileInfo if \a index is out of boundaries.

    \sa fileInfoList()
*/
UnpackZip::FileInfo UnpackZip::entryInfoAt(int index) const
{
    d->scanFiles();
    if (index >= 0 && index < d->fileHeaders.count())
        return d->fillFileInfo(index);
    return UnpackZip::FileInfo();
}

/*!
    Fetch the file contents from the zip archive and return the uncompressed bytes.
*/
QByteArray UnpackZip::fileData(const QString &fileName) const
{
    d->scanFiles();
    int i;
    for (i = 0; i < d->fileHeaders.size(); ++i) {
        if (QString::fromLocal8Bit(d->fileHeaders.at(i).file_name) == fileName)
            break;
    }
    if (i == d->fileHeaders.size())
        return QByteArray();

    FileHeader header = d->fileHeaders.at(i);

    ushort version_needed = readUShort(header.h.version_needed);
    if (version_needed > ZIP_VERSION) {
        qWarning("QZip: .ZIP specification version %d implementationis needed to extract the data.", version_needed);
        return QByteArray();
    }

    ushort general_purpose_bits = readUShort(header.h.general_purpose_bits);
    int compressed_size = readUInt(header.h.compressed_size);
    int uncompressed_size = readUInt(header.h.uncompressed_size);
    int start = readUInt(header.h.offset_local_header);
    //qDebug("uncompressing file %d: local header at %d", i, start);

    d->device->seek(start);
    LocalFileHeader lh;
    d->device->read((char *)&lh, sizeof(LocalFileHeader));
    uint skip = readUShort(lh.file_name_length) + readUShort(lh.extra_field_length);
    d->device->seek(d->device->pos() + skip);

    int compression_method = readUShort(lh.compression_method);
    //qDebug("file=%s: compressed_size=%d, uncompressed_size=%d", fileName.toLocal8Bit().data(), compressed_size, uncompressed_size);

    if ((general_purpose_bits & Encrypted) != 0) {
        qWarning("QZip: Unsupported encryption method is needed to extract the data.");
        return QByteArray();
    }

    //qDebug("file at %lld", d->device->pos());
    QByteArray compressed = d->device->read(compressed_size);
    if (compression_method == CompressionMethodStored) {
        // no compression
        compressed.truncate(uncompressed_size);
        return compressed;
    } else if (compression_method == CompressionMethodDeflated) {
        // Deflate
        //qDebug("compressed=%d", compressed.size());
        compressed.truncate(compressed_size);
        QByteArray baunzip;
        ulong len = qMax(uncompressed_size,  1);
        int res;
        do {
            baunzip.resize(len);
            res = inflate((uchar*)baunzip.data(), &len,
                          (const uchar*)compressed.constData(), compressed_size);

            switch (res) {
            case Z_OK:
                if ((int)len != baunzip.size())
                    baunzip.resize(len);
                break;
            case Z_MEM_ERROR:
                qWarning("QZip: Z_MEM_ERROR: Not enough memory");
                break;
            case Z_BUF_ERROR:
                len *= 2;
                break;
            case Z_DATA_ERROR:
                qWarning("QZip: Z_DATA_ERROR: Input data is corrupted");
                break;
            }
        } while (res == Z_BUF_ERROR);
        return baunzip;
    }

    qWarning("QZip: Unsupported compression method %d is needed to extract the data.", compression_method);
    return QByteArray();
}

/*!
    Extracts the full contents of the zip file into \a destinationDir on
    the local filesystem.
    In case writing or linking a file fails, the extraction will be aborted.
*/
bool UnpackZip::extractAll(const QString &destinationDir) const
{
    QDir baseDir(destinationDir);

    // create directories first
    const QVector<ZipFileInfo> allFiles = fileInfoList();
    for (const ZipFileInfo &fi : allFiles) {
        const QString absPath = destinationDir + QDir::separator() + fi.filePath;
        if (fi.isDir) {
            if (!baseDir.mkpath(fi.filePath))
                return false;
            if (!QFile::setPermissions(absPath, fi.permissions))
                return false;
        }
    }

    // set up symlinks
    for (const ZipFileInfo &fi : allFiles) {
        const QString absPath = destinationDir + QDir::separator() + fi.filePath;
        if (fi.isSymLink) {
            QString destination = QFile::decodeName(fileData(fi.filePath));
            if (destination.isEmpty())
                return false;
            QFileInfo linkFi(absPath);
            if (!QFile::exists(linkFi.absolutePath()))
                QDir::root().mkpath(linkFi.absolutePath());
            if (!QFile::link(destination, absPath))
                return false;
            /* cannot change permission of links
            if (!QFile::setPermissions(absPath, fi.permissions))
                return false;
            */
        }
    }

    for (const ZipFileInfo &fi : allFiles) {
        const QString absPath = destinationDir + QDir::separator() + fi.filePath;
        if (fi.isFile) {
            QFile f(absPath);
            if (!f.open(QIODevice::WriteOnly))
                return false;
            f.write(fileData(fi.filePath));
            f.setPermissions(fi.permissions);
            f.close();
        }
    }

    return true;
}

/*!
    \enum UnpackZip::Status

    The following status values are possible:

    \value NoError  No error occurred.
    \value FileReadError    An error occurred when reading from the file.
    \value FileOpenError    The file could not be opened.
    \value FilePermissionsError The file could not be accessed.
    \value FileError        Another file error occurred.
*/

/*!
    Returns a status code indicating the first error that was met by UnpackZip,
    or UnpackZip::NoError if no error occurred.
*/
Status UnpackZip::status() const
{
    return d->status;
}

/*!
    Close the zip file.
*/
void UnpackZip::close()
{
    d->device->close();
}
