// StormLibCxx (c) 2018 YarikTH
// This code is licensed under MIT license (see LICENSE for details)
#pragma once

#include <string>
#include <memory>

namespace StormLibCxx
{
    std::string getVersionString();

    using HANDLE = void*;
    using LCID = uint32_t;
    using path_t = std::string;


    using StormLibCxxArhiveOpenFlags = uint32_t;        ///< flags: for Archive::open()                     // enum StormLibCxxArhiveOpenFlags_
    using StormLibCxxArhiveCreateFlags = uint32_t;      ///< flags: for Archive::create()                   // enum StormLibCxxArhiveCreateFlags_
    using StormLibCxxArhiveAttributesFlags = uint32_t;  ///< flags: for Archive::getAttributes() etc.       // enum StormLibCxxArhiveAttributesFlags_
    using StormLibCxxFileSearhFlags = uint32_t;         ///< flags: for Archive::openFile() etc.            // enum StormLibCxxFileSearhFlags_

    /// flags: for Archive::open()
    enum StormLibCxxArhiveOpenFlags_
    {
        StormLibCxxArhiveOpenFlags_HardDiskFile     = 2,            ///< Open the archive on HDD
        StormLibCxxArhiveOpenFlags_CdRomFile        = 3,            ///< Open the archive only if it is on CDROM
        StormLibCxxArhiveOpenFlags_NoListfile       = 0x00010000,   ///< Don't load the internal listfile
        StormLibCxxArhiveOpenFlags_NoAttributes     = 0x00020000,   ///< Don't open the attributes
        StormLibCxxArhiveOpenFlags_NoHeaderSearch   = 0x00040000,   ///< Don't search for the MPQ header past the begin of the file
        StormLibCxxArhiveOpenFlags_ForceMpqV1       = 0x00080000,   ///< Always open the archive as MPQ v 1.00, ignore the "wFormatVersion" variable in the header
        StormLibCxxArhiveOpenFlags_SectorCrc        = 0x00100000,   ///< On files with MPQ_FILE_SECTOR_CRC, the CRC will be checked when reading file
        StormLibCxxArhiveOpenFlags_ReadOnly         = 0x00000100    ///< Open in read only mode
    };

    /// flags: for Archive::create()
    enum StormLibCxxArhiveCreateFlags_
    {
        StormLibCxxArhiveCreateFlags_Listfile       = 0x00100000,   ///< Also add the (listfile) file
        StormLibCxxArhiveCreateFlags_Attributes     = 0x00200000,   ///< Also add the (attributes) file
        StormLibCxxArhiveCreateFlags_Signature      = 0x00400000,   ///< Also add the (signature) file
        StormLibCxxArhiveCreateFlags_ArchiveV1      = 0x00000000,   ///< Creates archive of version 1 (size up to 4GB)
        StormLibCxxArhiveCreateFlags_ArchiveV2      = 0x01000000,   ///< Creates archive of version 2 (larger than 4 GB)
        StormLibCxxArhiveCreateFlags_ArchiveV3      = 0x02000000,   ///< Creates archive of version 3
        StormLibCxxArhiveCreateFlags_ArchiveV4      = 0x03000000    ///< Creates archive of version 4
    };

    /// flags: for Archive::getAttributes(), Archive::setAttributes()
    enum StormLibCxxArhiveAttributesFlags_
    {
        StormLibCxxArhiveAttributesFlags_Crc32      = 0x00000001,   ///< The "(attributes)" contains CRC32 for each file
        StormLibCxxArhiveAttributesFlags_Filetime   = 0x00000002,   ///< The "(attributes)" contains file time for each file
        StormLibCxxArhiveAttributesFlags_Md5        = 0x00000004,   ///< The "(attributes)" contains MD5 for each file
        StormLibCxxArhiveAttributesFlags_PatchBit   = 0x00000008,   ///< The "(attributes)" contains a patch bit for each file
        StormLibCxxArhiveAttributesFlags_All        = 0x0000000F,   ///< Summary mask
    };

    /// flags: for Archive::openFile() etc.
    enum StormLibCxxFileSearhFlags_
    {
        StormLibCxxFileSearhFlags_FromMpq           = 0x00000000,   ///< Open the file from the MPQ archive
        StormLibCxxFileSearhFlags_CheckExists       = 0xFFFFFFFC,   ///< Only check whether the file exists
        StormLibCxxFileSearhFlags_LocalFile         = 0xFFFFFFFF    ///< Open a local file
    };

    class File;
    class Find;

    class Archive
    {
    public:
        Archive();

        Archive( const path_t& mpqName, StormLibCxxArhiveOpenFlags flags = 0 );

        Archive( const Archive& ) = delete;
        Archive& operator=( const Archive& ) = delete;

        Archive( Archive&& );

        ~Archive();

        HANDLE getHandle() const;

        explicit operator bool() const;

        bool check() const;

        bool open( const path_t& mpqName, StormLibCxxArhiveOpenFlags flags = 0 );

        bool create( const path_t& mpqName, size_t maxFilesCount, StormLibCxxArhiveCreateFlags flags = 0 );

        bool isOpened() const;

        bool close();

        bool flush();

        bool addListFile( const path_t& listFile );

        size_t getMaxFileCount() const;

        bool setMaxFileCount( size_t value );

        StormLibCxxArhiveAttributesFlags getAttributes() const;

        bool setAttributes( StormLibCxxArhiveAttributesFlags value );

        bool updateFileAttributes( const path_t& fileName );

        bool openPatchArchive( const path_t& patchMpqName, const path_t& patchPathPrefix, StormLibCxxArhiveOpenFlags flags = 0 );

        bool isPatchedArchive() const;

        bool isFileExists( const path_t& fileName );

        File openFile( const path_t& fileName );

        bool extractFile( const path_t& toExtract, const path_t& extracted );

        Find findFiles( const std::string& mask, const path_t& listFile = "" );

        //bool   WINAPI CreateFile(HANDLE hMpq, const char * szArchivedName, ULONGLONG FileTime, DWORD dwFileSize, LCID lcLocale, DWORD dwFlags, HANDLE * phFile);

        //bool   WINAPI AddFile(HANDLE hMpq, const TCHAR * szFileName, const char * szArchivedName, DWORD dwFlags);
        //bool   WINAPI AddWave(HANDLE hMpq, const TCHAR * szFileName, const char * szArchivedName, DWORD dwFlags, DWORD dwQuality);
        //bool   WINAPI RemoveFile(HANDLE hMpq, const char * szFileName, DWORD dwSearchScope);
        //bool   WINAPI RenameFile(HANDLE hMpq, const char * szOldFileName, const char * szNewFileName);

    private:
        class Impl;

        std::unique_ptr<Impl> m_pimpl;
    };

    class File
    {
    private:
        friend class Archive;

        /// Open file from archive
        File( HANDLE archiveHandle, const path_t& fileName );

    public:
        /// Open local file
        File( const path_t& fileName );

        File( const Archive& ) = delete;
        File& operator=( const Archive& ) = delete;

        File( File&& );

        ~File();

        HANDLE getHandle() const;

        explicit operator bool() const;

        bool check() const;

        bool isOpened() const;

        bool close();

        uint64_t getFileSize() const;

        //DWORD  WINAPI SetFilePointer(HANDLE hFile, LONG lFilePos, LONG * plFilePosHigh, DWORD dwMoveMethod);
        //bool   WINAPI ReadFile(HANDLE hFile, void * lpBuffer, DWORD dwToRead, LPDWORD pdwRead, LPOVERLAPPED lpOverlapped);

        //bool   WINAPI CreateFile(HANDLE hMpq, const char * szArchivedName, ULONGLONG FileTime, DWORD dwFileSize, LCID lcLocale, DWORD dwFlags, HANDLE * phFile);
        //bool   WINAPI WriteFile(HANDLE hFile, const void * pvData, DWORD dwSize, DWORD dwCompression);
        //bool   WINAPI FinishFile(HANDLE hFile);

    private:
        class Impl;

        std::unique_ptr<Impl> m_pimpl;
    };

    class Find
    {
    private:
        friend class Archive;

        Find( HANDLE archiveHandle, const std::string& mask, const path_t& listFile );

    public:
        Find( const Find& ) = delete;
        Find& operator=( const Find& ) = delete;

        Find( Find&& );

        ~Find();

        HANDLE getHandle() const;

        explicit operator bool() const;

        bool check() const;

        bool isOpened() const;

        bool close();

        bool moveForward();

        Find& operator++();

        bool isFinished() const;

        std::string getFileName() const;

        std::string getPlainName() const;

        size_t getFileSize() const;

        size_t getFileFlags() const;

        size_t getCompSize() const;

        uint64_t getFileTime() const;

        LCID getLocale() const;

    private:
        class Impl;

        std::unique_ptr<Impl> m_pimpl;
    };

}