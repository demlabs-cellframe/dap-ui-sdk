#include "zipbase.h"
#include "zlib.h"
#include <QStringView>

namespace  DapZip {

    uint readUInt(const uchar *data)
    {
        return (data[0]) + (data[1]<<8) + (data[2]<<16) + (data[3]<<24);
    }

    ushort readUShort(const uchar *data)
    {
        return (data[0]) + (data[1]<<8);
    }

    void writeUInt(uchar *data, uint i)
    {
        data[0] = i & 0xff;
        data[1] = (i>>8) & 0xff;
        data[2] = (i>>16) & 0xff;
        data[3] = (i>>24) & 0xff;
    }

    void writeUShort(uchar *data, ushort i)
    {
        data[0] = i & 0xff;
        data[1] = (i>>8) & 0xff;
    }

    void copyUInt(uchar *dest, const uchar *src)
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        dest[3] = src[3];
    }

    void copyUShort(uchar *dest, const uchar *src)
    {
        dest[0] = src[0];
        dest[1] = src[1];
    }

    void writeMSDosDate(uchar *dest, const QDateTime& dt)
    {
        if (dt.isValid()) {
            quint16 time =
                (dt.time().hour() << 11)    // 5 bit hour
                | (dt.time().minute() << 5)   // 6 bit minute
                | (dt.time().second() >> 1);  // 5 bit double seconds

            dest[0] = time & 0xff;
            dest[1] = time >> 8;

            quint16 date =
                ((dt.date().year() - 1980) << 9) // 7 bit year 1980-based
                | (dt.date().month() << 5)           // 4 bit month
                | (dt.date().day());                 // 5 bit day

            dest[2] = char(date);
            dest[3] = char(date >> 8);
        } else {
            dest[0] = 0;
            dest[1] = 0;
            dest[2] = 0;
            dest[3] = 0;
        }
    }

    int inflate(Bytef *dest, ulong *destLen, const Bytef *source, ulong sourceLen)
    {
        z_stream stream;
        int err;

        stream.next_in = const_cast<Bytef*>(source);
        stream.avail_in = (uInt)sourceLen;
        if ((uLong)stream.avail_in != sourceLen)
            return Z_BUF_ERROR;

        stream.next_out = dest;
        stream.avail_out = (uInt)*destLen;
        if ((uLong)stream.avail_out != *destLen)
            return Z_BUF_ERROR;

        stream.zalloc = (alloc_func)nullptr;
        stream.zfree = (free_func)nullptr;

        err = inflateInit2(&stream, -MAX_WBITS);
        if (err != Z_OK)
            return err;

        err = inflate(&stream, Z_FINISH);
        if (err != Z_STREAM_END) {
            inflateEnd(&stream);
            if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
                return Z_DATA_ERROR;
            return err;
        }
        *destLen = stream.total_out;

        err = inflateEnd(&stream);
        return err;
    }

    int deflate (Bytef *dest, ulong *destLen, const Bytef *source, ulong sourceLen)
    {
        z_stream stream;
        int err;

        stream.next_in = const_cast<Bytef*>(source);
        stream.avail_in = (uInt)sourceLen;
        stream.next_out = dest;
        stream.avail_out = (uInt)*destLen;
        if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

        stream.zalloc = (alloc_func)nullptr;
        stream.zfree = (free_func)nullptr;
        stream.opaque = (voidpf)nullptr;

        err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
        if (err != Z_OK) return err;

        err = deflate(&stream, Z_FINISH);
        if (err != Z_STREAM_END) {
            deflateEnd(&stream);
            return err == Z_OK ? Z_BUF_ERROR : err;
        }
        *destLen = stream.total_out;

        err = deflateEnd(&stream);
        return err;
    }


    QFile::Permissions modeToPermissions(quint32 mode)
    {
        QFile::Permissions permissions;
        if (mode & UnixFileAttributes::ReadUser)
            permissions |= QFile::ReadOwner | QFile::ReadUser;
        if (mode & UnixFileAttributes::WriteUser)
            permissions |= QFile::WriteOwner | QFile::WriteUser;
        if (mode & UnixFileAttributes::ExeUser)
            permissions |= QFile::ExeOwner | QFile::ExeUser;
        if (mode & UnixFileAttributes::ReadGroup)
            permissions |= QFile::ReadGroup;
        if (mode & UnixFileAttributes::WriteGroup)
            permissions |= QFile::WriteGroup;
        if (mode & UnixFileAttributes::ExeGroup)
            permissions |= QFile::ExeGroup;
        if (mode & UnixFileAttributes::ReadOther)
            permissions |= QFile::ReadOther;
        if (mode & UnixFileAttributes::WriteOther)
            permissions |= QFile::WriteOther;
        if (mode & UnixFileAttributes::ExeOther)
            permissions |= QFile::ExeOther;
        return permissions;
    }

    quint32 permissionsToMode(QFile::Permissions perms)
    {
        quint32 mode = 0;
        if (perms & (QFile::ReadOwner | QFile::ReadUser))
            mode |= UnixFileAttributes::ReadUser;
        if (perms & (QFile::WriteOwner | QFile::WriteUser))
            mode |= UnixFileAttributes::WriteUser;
        if (perms & (QFile::ExeOwner | QFile::ExeUser))
            mode |= UnixFileAttributes::WriteUser;
        if (perms & QFile::ReadGroup)
            mode |= UnixFileAttributes::ReadGroup;
        if (perms & QFile::WriteGroup)
            mode |= UnixFileAttributes::WriteGroup;
        if (perms & QFile::ExeGroup)
            mode |= UnixFileAttributes::ExeGroup;
        if (perms & QFile::ReadOther)
            mode |= UnixFileAttributes::ReadOther;
        if (perms & QFile::WriteOther)
            mode |= UnixFileAttributes::WriteOther;
        if (perms & QFile::ExeOther)
            mode |= UnixFileAttributes::ExeOther;
        return mode;
    }

    QDateTime readMSDosDate(const uchar *src)
    {
        uint dosDate = readUInt(src);
        quint64 uDate;
        uDate = (quint64)(dosDate >> 16);
        uint tm_mday = (uDate & 0x1f);
        uint tm_mon =  ((uDate & 0x1E0) >> 5);
        uint tm_year = (((uDate & 0x0FE00) >> 9) + 1980);
        uint tm_hour = ((dosDate & 0xF800) >> 11);
        uint tm_min =  ((dosDate & 0x7E0) >> 5);
        uint tm_sec =  ((dosDate & 0x1f) << 1);

        return QDateTime(QDate(tm_year, tm_mon, tm_mday), QTime(tm_hour, tm_min, tm_sec));
    }


    LocalFileHeader toLocalHeader(const CentralFileHeader &ch)
    {
        LocalFileHeader h;
        writeUInt(h.signature, LocalFileHeaderSignature);
        copyUShort(h.version_needed, ch.version_needed);
        copyUShort(h.general_purpose_bits, ch.general_purpose_bits);
        copyUShort(h.compression_method, ch.compression_method);
        copyUInt(h.last_mod_file, ch.last_mod_file);
        copyUInt(h.crc_32, ch.crc_32);
        copyUInt(h.compressed_size, ch.compressed_size);
        copyUInt(h.uncompressed_size, ch.uncompressed_size);
        copyUShort(h.file_name_length, ch.file_name_length);
        copyUShort(h.extra_field_length, ch.extra_field_length);
        return h;
    }
}

using namespace DapZip;


ZipFileInfo ZipBase::fillFileInfo(int index) const
{
    ZipFileInfo fileInfo;
    FileHeader header = fileHeaders.at(index);
    quint32 mode = readUInt(header.h.external_file_attributes);
    const HostOS hostOS = HostOS(readUShort(header.h.version_made) >> 8);
    switch (hostOS) {
    case HostUnix:
        mode = (mode >> 16) & 0xffff;
        switch (mode & UnixFileAttributes::TypeMask) {
        case UnixFileAttributes::SymLink:
            fileInfo.isSymLink = true;
            break;
        case UnixFileAttributes::Dir:
            fileInfo.isDir = true;
            break;
        case UnixFileAttributes::File:
        default: // ### just for the case; should we warn?
            fileInfo.isFile = true;
            break;
        }
        fileInfo.permissions = modeToPermissions(mode);
        break;
    case HostFAT:
    case HostNTFS:
    case HostHPFS:
    case HostVFAT:
        switch (mode & WindowsFileAttributes::TypeMask) {
        case WindowsFileAttributes::Dir:
            fileInfo.isDir = true;
            break;
        case WindowsFileAttributes::File:
        default:
            fileInfo.isFile = true;
            break;
        }
        fileInfo.permissions |= QFile::ReadOwner | QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther;
        if ((mode & WindowsFileAttributes::ReadOnly) == 0)
            fileInfo.permissions |= QFile::WriteOwner | QFile::WriteUser | QFile::WriteGroup | QFile::WriteOther;
        if (fileInfo.isDir)
            fileInfo.permissions |= QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup | QFile::ExeOther;
        break;
    default:
        qWarning("DapZip: Zip entry format at %d is not supported.", index);
        return fileInfo; // we don't support anything else
    }

    ushort general_purpose_bits = readUShort(header.h.general_purpose_bits);
    // if bit 11 is set, the filename and comment fields must be encoded using UTF-8
    const bool inUtf8 = (general_purpose_bits & Utf8Names) != 0;
    fileInfo.filePath = inUtf8 ? QString::fromUtf8(header.file_name) : QString::fromLocal8Bit(header.file_name);
    fileInfo.crc = readUInt(header.h.crc_32);
    fileInfo.size = readUInt(header.h.uncompressed_size);
    fileInfo.lastModified = readMSDosDate(header.h.last_mod_file);

    // fix the file path, if broken (convert separators, eat leading and trailing ones)
    fileInfo.filePath = QDir::fromNativeSeparators(fileInfo.filePath);
    QStringView filePathRef(fileInfo.filePath);
    while (filePathRef.startsWith(QLatin1Char('.')) || filePathRef.startsWith(QLatin1Char('/')))
        filePathRef = filePathRef.mid(1);
    while (filePathRef.endsWith(QLatin1Char('/')))
        filePathRef = filePathRef.chopped(1);

    fileInfo.filePath = filePathRef.toString();
    return fileInfo;
}
