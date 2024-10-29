#include "packzip.h"

#define ICOMPRESSIBLE_LENGTH 64

using namespace DapZip;

void PackZipPrivate::addEntry(EntryType type, const QString &fileName, const QByteArray &contents)
{
    static const char *const entryTypes[] = {
        "directory",
        "file     ",
        "symlink  " };
    qDebug() << "adding" << entryTypes[type] <<":" << fileName.toUtf8().data() << (type == 2 ? QByteArray(" -> " + contents).constData() : "");

    if (! (device->isOpen() || device->open(QIODevice::WriteOnly)))
    {
        status = Status::FileOpenError;
        return;
    }
    device->seek(start_of_directory);

    // don't compress small files
    DapZip::CompressionPolicy compression = compressionPolicy;
    if (compressionPolicy == DapZip::AutoCompress)
    {
        if (contents.length() < ICOMPRESSIBLE_LENGTH)
            compression = DapZip::NeverCompress;
        else
            compression = DapZip::AlwaysCompress;
    }

    FileHeader header;
    // fill header
    memset(&header.h, 0, sizeof(CentralFileHeader));
    writeUInt(header.h.signature, CentralFileHeaderSignature);

    writeUShort(header.h.version_needed, ZIP_VERSION);
    writeUInt(header.h.uncompressed_size, contents.length());
    writeMSDosDate(header.h.last_mod_file, QDateTime::currentDateTime());
    QByteArray data = contents;
    if (compression == DapZip::AlwaysCompress)
    {
        // compress data
        writeUShort(header.h.compression_method, CompressionMethodDeflated);

        ulong len = contents.length();
        // copied form zlib
        len += (len >> 12) + (len >> 14) + 11;
        int res;
        do
        {
            data.resize(len);
            res = deflate((uchar*)data.data(), &len, (const uchar*)contents.constData(), contents.length());

            switch (res)
            {
                case Z_OK:
                    data.resize(len);
                    break;
                case Z_MEM_ERROR:
                    qWarning("DapZip: Z_MEM_ERROR: Not enough memory to compress file, skipping");
                    data.resize(0);
                    break;
                case Z_BUF_ERROR:
                    len *= 2;
                    break;
            }
        } while (res == Z_BUF_ERROR);
    }
    writeUInt(header.h.compressed_size, data.length());
    // crc
    uint crc_32 = ::crc32(0, nullptr, 0);
    crc_32 = ::crc32(crc_32, (const uchar *)contents.constData(), contents.length());
    writeUInt(header.h.crc_32, crc_32);

    // if bit 11 is set, the filename and comment fields must be encoded using UTF-8
    ushort general_purpose_bits = Utf8Names; // always use utf-8
    writeUShort(header.h.general_purpose_bits, general_purpose_bits);

    const bool inUtf8 = (general_purpose_bits & Utf8Names) != 0;
    header.file_name = inUtf8 ? fileName.toUtf8() : fileName.toLocal8Bit();
    // fill filename
    if (header.file_name.size() > 0xffff)
    {
        qWarning("DapQZip: Filename is too long, chopping it to 65535 bytes");
        header.file_name = header.file_name.left(0xffff); // ### don't break the utf-8 sequence, if any
    }
    if (header.file_comment.size() + header.file_name.size() > 0xffff)
    {
        qWarning("DapZip: File comment is too long, chopping it to 65535 bytes");
        header.file_comment.truncate(0xffff - header.file_name.size()); // ### don't break the utf-8 sequence, if any
    }
    writeUShort(header.h.file_name_length, header.file_name.length());
    //h.extra_field_length[2];

    writeUShort(header.h.version_made, HostUnix << 8);
    //uchar internal_file_attributes[2];
    //uchar external_file_attributes[4];
    quint32 mode = permissionsToMode(permissions);
    // entry type
    switch (type)
    {
        case Symlink:
            mode |= UnixFileAttributes::SymLink;
            break;
        case Directory:
            mode |= UnixFileAttributes::Dir;
            break;
        case File:
            mode |= UnixFileAttributes::File;
            break;
        default:
            Q_UNREACHABLE();
            break;
    }
    writeUInt(header.h.external_file_attributes, mode << 16);
    writeUInt(header.h.offset_local_header, start_of_directory);


    fileHeaders.append(header);

    LocalFileHeader h = toLocalHeader(header.h);
    device->write((const char *)&h, sizeof(LocalFileHeader));
    device->write(header.file_name);
    device->write(data);
    start_of_directory = device->pos();
    dirtyFileTree = true;
}


PackZip::PackZip(const QString &fileName, QIODevice::OpenMode mode)
{
    QScopedPointer<QFile> f(new QFile(fileName));
    DapZip::Status status;
    if (f->open(mode) && f->error() == QFile::NoError)
        status = DapZip::NoError;
    else {
        if (f->error() == QFile::WriteError)
            status = DapZip::FileWriteError;
        else if (f->error() == QFile::OpenError)
            status = DapZip::FileOpenError;
        else if (f->error() == QFile::PermissionsError)
            status = DapZip::FilePermissionsError;
        else
            status = DapZip::FileError;
    }

    d = new PackZipPrivate(f.data(), /*ownDevice=*/true);
    f.take();
    d->status = status;
}

PackZip::PackZip(QIODevice *device)
    : d(new PackZipPrivate(device, /*ownDevice=*/false))
{
    Q_ASSERT(device);
}

PackZip::~PackZip()
{
    close();
    delete d;
}

/*!
    Add a file to the archive.
    The file will be stored in the root of the archive using the fileName of file.

    The new file will get the file permissions based on the current
    creationPermissions and it will be compressed using the zip compression
    based on the current compression policy.
*/

void PackZip::addFile(const QString &filePath)
{
    QFile file(filePath);
    QFileInfo finfo(filePath);
    if(file.exists())
    {
        file.open(QIODevice::ReadOnly);
        addFile(finfo.fileName(), file.readAll());
        file.close();
    }
}

/*!
    Add a file to the archive with data as the file contents.
    The file will be stored in the archive using the fileName which
    includes the full path in the archive.

    The new file will get the file permissions based on the current
    creationPermissions and it will be compressed using the zip compression
    based on the current compression policy.
*/
void PackZip::addFile(const QString &fileName, const QByteArray &data)
{
    d->addEntry(PackZipPrivate::File, QDir::fromNativeSeparators(fileName), data);
}

/*!
    Add a file to the archive with device as the source of the contents.
    The contents returned from QIODevice::readAll() will be used as the
    filedata.
    The file will be stored in the archive using the fileName which
    includes the full path in the archive.
*/

void PackZip::addFile(const QString &fileName, QIODevice *device)
{
    Q_ASSERT(device);
    QIODevice::OpenMode mode = device->openMode();
    bool opened = false;
    if ((mode & QIODevice::ReadOnly) == 0)
    {
        opened = true;
        if (! device->open(QIODevice::ReadOnly))
        {
            d->status = FileOpenError;
            return;
        }
    }
    d->addEntry(PackZipPrivate::File, QDir::fromNativeSeparators(fileName), device->readAll());
    if (opened)
        device->close();
}

/*!
    Create a new directory in the archive with the specified dirName and
    the permissions;
*/
void PackZip::addDirectory(const QString &dirName)
{
    QString name(QDir::fromNativeSeparators(dirName));
    // separator is mandatory
    if (!name.endsWith(QLatin1Char('/')))
        name.append(QLatin1Char('/'));
    d->addEntry(PackZipPrivate::Directory, name, QByteArray());
}

/*!
    Create a new symbolic link in the archive with the specified dirName
    and the permissions;
    A symbolic link contains the destination (relative) path and name.
*/
void PackZip::addSymLink(const QString &fileName, const QString &destination)
{
    d->addEntry(PackZipPrivate::Symlink, QDir::fromNativeSeparators(fileName), QFile::encodeName(destination));
}

/*!
   Closes the zip file.
*/
void PackZip::close()
{
    if (!(d->device->openMode() & QIODevice::WriteOnly))
    {
        d->device->close();
        return;
    }

    d->device->seek(d->start_of_directory);
    // write new directory
    for (int i = 0; i < d->fileHeaders.size(); ++i) {
        const FileHeader &header = d->fileHeaders.at(i);
        d->device->write((const char *)&header.h, sizeof(CentralFileHeader));
        d->device->write(header.file_name);
        d->device->write(header.extra_field);
        d->device->write(header.file_comment);
    }
    int dir_size = d->device->pos() - d->start_of_directory;
    // write end of directory
    EndOfDirectory eod;
    memset(&eod, 0, sizeof(EndOfDirectory));
    writeUInt(eod.signature, EndOfDirectorySignature);
    //uchar this_disk[2];
    //uchar start_of_directory_disk[2];
    writeUShort(eod.num_dir_entries_this_disk, d->fileHeaders.size());
    writeUShort(eod.num_dir_entries, d->fileHeaders.size());
    writeUInt(eod.directory_size, dir_size);
    writeUInt(eod.dir_start_offset, d->start_of_directory);
    writeUShort(eod.comment_length, d->comment.length());

    d->device->write((const char *)&eod, sizeof(EndOfDirectory));
    d->device->write(d->comment);
    d->device->close();
}

/*!
    Returns device used for reading zip archive.
*/
QIODevice* PackZip::device() const
{
    return d->device;
}

/*!
    Returns true if the file exists; otherwise returns false.
*/
bool PackZip::exists() const
{
    QFile *f = qobject_cast<QFile*> (d->device);
    if (f == nullptr)
        return true;
    return f->exists();
}

/*!
    Returns a status code indicating the first error
*/

Status PackZip::status() const
{
    return d->status;
}

/*!
    Sets the policy for compressing newly added files to the new policy.
    The default policy is AlwaysCompress
*/
void PackZip::setCompressionPolicy(CompressionPolicy policy)
{
    d->compressionPolicy = policy;
}

/*!
     Returns the currently set compression policy.
*/
CompressionPolicy PackZip::compressionPolicy() const
{
    return d->compressionPolicy;
}

/*!
    Sets the permissions that will be used for newly added files.
    The default permissions are QFile::ReadOwner | QFile::WriteOwner.
*/


void PackZip::setCreationPermissions(QFile::Permissions permissions)
{
    d->permissions = permissions;
}

/*!
     Returns the currently set creation permissions.
*/
QFile::Permissions PackZip::creationPermissions() const
{
    return d->permissions;
}


namespace DapZip
{
    bool compressFiles(const QString archive, const QStringList fileList)
    {
        PackZip zip(archive);
        if (zip.status() != Status::NoError)
        {
            qWarning() << "DapZip: create archive error";
            zip.close();
            return false;
        }
        foreach (QString file , fileList)
        {
            zip.addFile(file);
            if (zip.status() == Status::FileOpenError)
            {
                qWarning() << "DapZip: unable to create archive the file" << file << "cannot be opened for reading";
                zip.close();
                QFile::remove(archive);
                return false;
            }
        }
        zip.close();
        return true;
    }
}
