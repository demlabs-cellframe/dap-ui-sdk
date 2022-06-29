#ifndef ZIPDEF_H
#define ZIPDEF_H

#include <QtCore>

// Zip standard version for archives handled by this API
// the only basic support
#define ZIP_VERSION 20

namespace WindowsFileAttributes
{
    enum
    {
        Dir        = 0x10, // FILE_ATTRIBUTE_DIRECTORY
        File       = 0x80, // FILE_ATTRIBUTE_NORMAL
        TypeMask   = 0x90,

        ReadOnly   = 0x01, // FILE_ATTRIBUTE_READONLY
        PermMask   = 0x01
    };
}

namespace UnixFileAttributes
{
    enum
    {
        Dir        = 0040000, // __S_IFDIR
        File       = 0100000, // __S_IFREG
        SymLink    = 0120000, // __S_IFLNK
        TypeMask   = 0170000, // __S_IFMT

        ReadUser   = 0400, // __S_IRUSR
        WriteUser  = 0200, // __S_IWUSR
        ExeUser    = 0100, // __S_IXUSR
        ReadGroup  = 0040, // __S_IRGRP
        WriteGroup = 0020, // __S_IWGRP
        ExeGroup   = 0010, // __S_IXGRP
        ReadOther  = 0004, // __S_IROTH
        WriteOther = 0002, // __S_IWOTH
        ExeOther   = 0001, // __S_IXOTH
        PermMask   = 0777
    };
}

namespace DapZip
{
    enum HostOS
    {
        HostFAT      = 0,
        HostAMIGA    = 1,
        HostVMS      = 2,  // VAX/VMS
        HostUnix     = 3,
        HostVM_CMS   = 4,
        HostAtari    = 5,  // what if it's a minix filesystem? [cjh]
        HostHPFS     = 6,  // filesystem used by OS/2 (and NT 3.x)
        HostMac      = 7,
        HostZ_System = 8,
        HostCPM      = 9,
        HostTOPS20   = 10, // pkzip 2.50 NTFS
        HostNTFS     = 11, // filesystem used by Windows NT
        HostQDOS     = 12, // SMS/QDOS
        HostAcorn    = 13, // Archimedes Acorn RISC OS
        HostVFAT     = 14, // filesystem used by Windows 95, NT
        HostMVS      = 15,
        HostBeOS     = 16, // hybrid POSIX/database filesystem
        HostTandem   = 17,
        HostOS400    = 18,
        HostOSX      = 19
    };

    enum GeneralPurposeFlag
    {
        Encrypted = 0x01,
        AlgTune1 = 0x02,
        AlgTune2 = 0x04,
        HasDataDescriptor = 0x08,
        PatchedData = 0x20,
        StrongEncrypted = 0x40,
        Utf8Names = 0x0800,
        CentralDirectoryEncrypted = 0x2000
    };

    enum CompressionMethod
    {
        CompressionMethodStored = 0,
        CompressionMethodShrunk = 1,
        CompressionMethodReduced1 = 2,
        CompressionMethodReduced2 = 3,
        CompressionMethodReduced3 = 4,
        CompressionMethodReduced4 = 5,
        CompressionMethodImploded = 6,
        CompressionMethodReservedTokenizing = 7, // reserved for tokenizing
        CompressionMethodDeflated = 8,
        CompressionMethodDeflated64 = 9,
        CompressionMethodPKImploding = 10,

        CompressionMethodBZip2 = 12,

        CompressionMethodLZMA = 14,

        CompressionMethodTerse = 18,
        CompressionMethodLz77 = 19,

        CompressionMethodJpeg = 96,
        CompressionMethodWavPack = 97,
        CompressionMethodPPMd = 98,
        CompressionMethodWzAES = 99
    };

    struct LocalFileHeader
    {
        uchar signature[4]; //  LocalFileHeaderSignature
        uchar version_needed[2];
        uchar general_purpose_bits[2];
        uchar compression_method[2];
        uchar last_mod_file[4];
        uchar crc_32[4];
        uchar compressed_size[4];
        uchar uncompressed_size[4];
        uchar file_name_length[2];
        uchar extra_field_length[2];
    };

    struct DataDescriptor
    {
        uchar crc_32[4];
        uchar compressed_size[4];
        uchar uncompressed_size[4];
    };

    struct CentralFileHeader
    {
        uchar signature[4]; // CentralFileHeaderSignature
        uchar version_made[2];
        uchar version_needed[2];
        uchar general_purpose_bits[2];
        uchar compression_method[2];
        uchar last_mod_file[4];
        uchar crc_32[4];
        uchar compressed_size[4];
        uchar uncompressed_size[4];
        uchar file_name_length[2];
        uchar extra_field_length[2];
        uchar file_comment_length[2];
        uchar disk_start[2];
        uchar internal_file_attributes[2];
        uchar external_file_attributes[4];
        uchar offset_local_header[4];
    };

    struct EndOfDirectory
    {
        uchar signature[4]; // EndOfDirectorySignature
        uchar this_disk[2];
        uchar start_of_directory_disk[2];
        uchar num_dir_entries_this_disk[2];
        uchar num_dir_entries[2];
        uchar directory_size[4];
        uchar dir_start_offset[4];
        uchar comment_length[2];
    };

    struct FileHeader
    {
        CentralFileHeader h;
        QByteArray file_name;
        QByteArray extra_field;
        QByteArray file_comment;
    };

    enum Signature
    {
        LocalFileHeaderSignature = 0x04034b50,      // Local file header signature
        CentralFileHeaderSignature = 0x02014b50,    // Central file header signature
        EndOfDirectorySignature = 0x06054b50        // End of directory signature
    };

}

#endif // ZIPDEF_H
