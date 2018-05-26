// StormLibCxx (c) 2018 YarikTH
// This code is licensed under MIT license (see LICENSE for details)
#pragma once

#include "StormLibCxx/StormLibCxx.h"
#include "StormLib/StormLib.h"

namespace StormLibCxx
{

    class Archive::Impl
    {
    public:
        Impl();

        Impl( const path_t& path, StormLibCxxArhiveOpenFlags flags );

        Impl( const Impl& ) = delete;
        Impl& operator=( const Impl& ) = delete;

        ~Impl();

        HANDLE getHandle() const;

        bool check() const;

        bool open( const path_t& path, StormLibCxxArhiveOpenFlags flags );

        bool create( const path_t& path, size_t maxFilesCount, StormLibCxxArhiveCreateFlags flags );

        bool isOpened() const;

        bool close();

        bool flush();

        bool addListFile( const path_t& path );

        size_t getMaxFileCount() const;

        bool setMaxFileCount( size_t value );

        StormLibCxxArhiveAttributesFlags getAttributes() const;

        bool setAttributes( StormLibCxxArhiveAttributesFlags value );

        bool updateFileAttributes( const path_t& path );

        bool openPatchArchive( const path_t& path, const path_t& patchPathPrefix, StormLibCxxArhiveOpenFlags flags );

        bool isPatchedArchive() const;

        bool isFileExists( const path_t& fileName );

        File openFile( const path_t& fileName );

        //bool   WINAPI GetFileInfo(HANDLE hMpqOrFile, InfoClass InfoClass, void * pvFileInfo, DWORD cbFileInfo, LPDWORD pcbLengthNeeded);
        //bool   WINAPI FreeFileInfo(void * pvFileInfo, InfoClass InfoClass);

        bool extractFile( const path_t& toExtract, const path_t& extracted );

        Find findFiles( const std::string& mask, const path_t& listFile );

        //bool   WINAPI CreateFile(HANDLE hMpq, const char * szArchivedName, ULONGLONG FileTime, DWORD dwFileSize, LCID lcLocale, DWORD dwFlags, HANDLE * phFile);

        //bool   WINAPI AddFileEx(HANDLE hMpq, const TCHAR * szFileName, const char * szArchivedName, DWORD dwFlags, DWORD dwCompression, DWORD dwCompressionNext);
        //bool   WINAPI AddFile(HANDLE hMpq, const TCHAR * szFileName, const char * szArchivedName, DWORD dwFlags);
        //bool   WINAPI AddWave(HANDLE hMpq, const TCHAR * szFileName, const char * szArchivedName, DWORD dwFlags, DWORD dwQuality);
        //bool   WINAPI RemoveFile(HANDLE hMpq, const char * szFileName, DWORD dwSearchScope);
        //bool   WINAPI RenameFile(HANDLE hMpq, const char * szOldFileName, const char * szNewFileName);

    private:
        HANDLE m_handle = nullptr;
        mutable DWORD m_lastError = ERROR_SUCCESS;
    };


    class File::Impl
    {
    public:
        Impl( HANDLE archiveHandle, const path_t& fileName );

        Impl( const path_t& fileName );

        Impl( const Impl& ) = delete;
        Impl& operator=( const Impl& ) = delete;

        ~Impl();

        HANDLE getHandle() const;

        bool check() const;

        bool isOpened() const;

        bool close();

        uint64_t getFileSize() const;
        //DWORD  WINAPI SetFilePointer(HANDLE hFile, LONG lFilePos, LONG * plFilePosHigh, DWORD dwMoveMethod);
        //bool   WINAPI ReadFile(HANDLE hFile, void * lpBuffer, DWORD dwToRead, LPDWORD pdwRead, LPOVERLAPPED lpOverlapped);

        //bool   WINAPI GetFileInfo(HANDLE hMpqOrFile, InfoClass InfoClass, void * pvFileInfo, DWORD cbFileInfo, LPDWORD pcbLengthNeeded);
        //bool   WINAPI GetFileName(HANDLE hFile, char * szFileName);
        //bool   WINAPI FreeFileInfo(void * pvFileInfo, InfoClass InfoClass);

        //bool   WINAPI CreateFile(HANDLE hMpq, const char * szArchivedName, ULONGLONG FileTime, DWORD dwFileSize, LCID lcLocale, DWORD dwFlags, HANDLE * phFile);
        //bool   WINAPI WriteFile(HANDLE hFile, const void * pvData, DWORD dwSize, DWORD dwCompression);
        //bool   WINAPI FinishFile(HANDLE hFile);

    private:
        HANDLE m_handle = nullptr;
        mutable DWORD m_lastError = ERROR_SUCCESS;
    };

    class Find::Impl
    {
    public:
        Impl( HANDLE archiveHandle, const std::string& mask, const path_t& listFile );

        Impl( const Impl& ) = delete;
        Impl& operator=( const Impl& ) = delete;

        ~Impl();

        HANDLE getHandle() const;

        bool check() const;

        bool isOpened() const;

        bool close();

        bool moveForward();

        bool isFinished() const;

        std::string getFileName() const;

        std::string getPlainName() const;

        size_t getFileSize() const;

        size_t getFileFlags() const;

        size_t getCompSize() const;

        uint64_t getFileTime() const;

        LCID getLocale() const;

    private:
        HANDLE m_handle = nullptr;
        mutable DWORD m_lastError = ERROR_SUCCESS;
        bool m_finished = false;
        SFILE_FIND_DATA m_data = {};
    };
}