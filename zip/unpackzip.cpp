#include "unpackzip.h"

using namespace DapZip;


void UnpackZipPrivate::scanFiles()
{
    if (!dirtyFileTree)
        return;

    if (! (device->isOpen() || device->open(QIODevice::ReadOnly))) {
        status = FileOpenError;
        return;
    }

    if ((device->openMode() & QIODevice::ReadOnly) == 0) { // only read the index from readable files.
        status = FileReadError;
        return;
    }

    dirtyFileTree = false;
    uchar tmp[4];
    device->read((char *)tmp, 4);
    if (readUInt(tmp) != LocalFileHeaderSignature) {
        qWarning("DapZip: not a zip file!");
        return;
    }

    // find EndOfDirectory header
    int i = 0;
    int start_of_directory = -1;
    int num_dir_entries = 0;
    EndOfDirectory eod;
    while (start_of_directory == -1) {
        const int pos = device->size() - int(sizeof(EndOfDirectory)) - i;
        if (pos < 0 || i > 65535) {
            qWarning("DapZip: EndOfDirectory not found");
            return;
        }

        device->seek(pos);
        device->read((char *)&eod, sizeof(EndOfDirectory));
        if (readUInt(eod.signature) == EndOfDirectorySignature)
            break;
        ++i;
    }

    // have the eod
    start_of_directory = readUInt(eod.dir_start_offset);
    num_dir_entries = readUShort(eod.num_dir_entries);
    qDebug("start_of_directory at %d, num_dir_entries=%d", start_of_directory, num_dir_entries);
    int comment_length = readUShort(eod.comment_length);
    if (comment_length != i)
        qWarning("DapZip: failed to parse zip file.");
    comment = device->read(qMin(comment_length, i));


    device->seek(start_of_directory);
    for (i = 0; i < num_dir_entries; ++i) {
        FileHeader header;
        int read = device->read((char *) &header.h, sizeof(CentralFileHeader));
        if (read < (int)sizeof(CentralFileHeader)) {
            qWarning("DapZip: Failed to read complete header, index may be incomplete");
            break;
        }
        if (readUInt(header.h.signature) != CentralFileHeaderSignature) {
            qWarning("DapZip: invalid header signature, index may be incomplete");
            break;
        }

        int l = readUShort(header.h.file_name_length);
        header.file_name = device->read(l);
        if (header.file_name.length() != l) {
            qWarning("DapZip: Failed to read filename from zip index, index may be incomplete");
            break;
        }
        l = readUShort(header.h.extra_field_length);
        header.extra_field = device->read(l);
        if (header.extra_field.length() != l) {
            qWarning("DapZip: Failed to read extra field in zip file, skipping file, index may be incomplete");
            break;
        }
        l = readUShort(header.h.file_comment_length);
        header.file_comment = device->read(l);
        if (header.file_comment.length() != l) {
            qWarning("DapZip: Failed to read read file comment, index may be incomplete");
            break;
        }

        qDebug("found file '%s'", header.file_name.data());
        fileHeaders.append(header);
    }
}


/*!
    UnpackZip

    The UnpackZip class provides a way to inspect the contents of a zip
    archive and extract individual files from it.

    UnpackZip can be used to read a zip archive either from a file or from any
    device. An in-memory QBuffer for instance.  The reader can be used to read
    which files are in the archive using fileInfoList() and entryInfoAt() but
    also to extract individual files using fileData() or even to extract all
    files in the archive using extractAll()
*/

/*!
    Create a new zip archive that operates on the fileName.  The file will be
    opened with the mode.
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
    Create a new zip archive that operates on the archive found in device.
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
    Returns true if the user can read the file; otherwise returns false.
*/
bool UnpackZip::isReadable() const
{
    return d->device->isReadable();
}

/*!
    Returns true if the file exists; otherwise returns false.
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
    Returns an invalid FileInfo if index is out of boundaries.
*/

ZipFileInfo UnpackZip::entryInfoAt(int index) const
{
    d->scanFiles();
    if (index >= 0 && index < d->fileHeaders.count())
        return d->fillFileInfo(index);
    return ZipFileInfo();
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
        qWarning("DapZip: .ZIP specification version %d implementationis needed to extract the data.", version_needed);
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
        qWarning("DapZip: Unsupported encryption method is needed to extract the data.");
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
                qWarning("DapZip: Z_MEM_ERROR: Not enough memory");
                break;
            case Z_BUF_ERROR:
                len *= 2;
                break;
            case Z_DATA_ERROR:
                qWarning("DapZip: Z_DATA_ERROR: Input data is corrupted");
                break;
            }
        } while (res == Z_BUF_ERROR);
        return baunzip;
    }

    qWarning("DapZip: Unsupported compression method %d is needed to extract the data.", compression_method);
    return QByteArray();
}

/*!
    Extracts the full contents of the zip file into destinationDir on
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
    The following status values are possible:

    NoError  No error occurred.
    FileReadError    An error occurred when reading from the file.
    FileOpenError    The file could not be opened.
    FilePermissionsError The file could not be accessed.
    FileError        Another file error occurred.
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

namespace DapZip {
    bool fileDecompression(const QString archiveFile, const QString &destinationDir)
    {
        UnpackZip zip(archiveFile);
        if (zip.status() != Status::NoError)
        {
            qWarning() << "DapZip: open archive error";
            zip.close();
            return false;
        }

        QDir baseDir(destinationDir);
        baseDir.mkpath(".");
        return zip.extractAll(destinationDir);
    }
}

