// StormLibCxx (c) 2018 YarikTH
// This code is licensed under MIT license (see LICENSE for details)

#include "StormLibCxxImpl.h"

namespace StormLibCxx
{
    Archive::Impl::Impl()
    {
    }

    Archive::Impl::Impl( const path_t& path, StormLibCxxArhiveOpenFlags flags )
    {
        open( path, flags );
    }

    Archive::Impl::~Impl()
    {
        close();
    }

    HANDLE Archive::Impl::getHandle() const
    {
        return m_handle;
    }

    bool Archive::Impl::check() const
    {
        return m_handle != nullptr 
            && m_lastError == ERROR_SUCCESS;
    }

    bool Archive::Impl::open( const path_t& path, StormLibCxxArhiveOpenFlags flags )
    {
        assert( !isOpened() );

        DWORD priority = 0; // It seems that priority isn't used for SFileOpenArchive

        m_lastError = ERROR_SUCCESS;
        bool result = SFileOpenArchive( path.c_str(), priority, static_cast<DWORD>( flags ), &m_handle );
        m_lastError = GetLastError();
        return result;
    }

    bool Archive::Impl::create( const path_t& path, size_t maxFilesCount, StormLibCxxArhiveCreateFlags flags )
    {
        assert( !isOpened() );

        m_lastError = ERROR_SUCCESS;
        bool result = SFileCreateArchive( path.c_str(), static_cast<DWORD>( flags ), static_cast<DWORD>( maxFilesCount ), &m_handle );
        m_lastError = GetLastError();
        return result;
    }

    bool Archive::Impl::isOpened() const
    {
        return m_handle != nullptr;
    }

    bool Archive::Impl::close()
    {
        if( m_handle )
        {
            bool result = SFileCloseArchive( m_handle );
            m_lastError = GetLastError();
            m_handle = nullptr;
            return result;
        }
        else
        {
            return false;
        }
    }

    bool Archive::Impl::flush()
    {
        assert( isOpened() );

        bool result = SFileFlushArchive( m_handle );
        m_lastError = GetLastError();
        return result;
    }

    bool Archive::Impl::addListFile( const path_t& path )
    {
        assert( isOpened() );

        m_lastError = SFileAddListFile( m_handle, path.c_str() );

        return m_lastError == ERROR_SUCCESS;
    }

    size_t Archive::Impl::getMaxFileCount() const
    {
        assert( isOpened() );

        size_t result = static_cast<size_t>( SFileGetMaxFileCount( m_handle ) );
        m_lastError = GetLastError();
        return result;
    }

    bool Archive::Impl::setMaxFileCount( size_t value )
    {
        assert( isOpened() );

        bool result = SFileSetMaxFileCount( m_handle, static_cast<DWORD>( value ) );
        m_lastError = GetLastError();
        return result;
    }

    StormLibCxxArhiveAttributesFlags Archive::Impl::getAttributes() const
    {
        assert( isOpened() );

        DWORD result = SFileGetAttributes( m_handle );
        m_lastError = GetLastError();

        return static_cast<StormLibCxxArhiveAttributesFlags>( result );
    }

    bool Archive::Impl::setAttributes( StormLibCxxArhiveAttributesFlags value )
    {
        assert( isOpened() );

        bool result = SFileSetAttributes( m_handle, static_cast<DWORD>( value ) );
        m_lastError = GetLastError();
        return result;
    }

    bool Archive::Impl::updateFileAttributes( const path_t& path )
    {
        assert( isOpened() );

        bool result = SFileUpdateFileAttributes( m_handle, path.c_str() );
        m_lastError = GetLastError();
        return result;
    }

    bool Archive::Impl::openPatchArchive( const path_t& path, const path_t& patchPathPrefix, StormLibCxxArhiveOpenFlags flags )
    {
        assert( isOpened() );

        bool result = SFileOpenPatchArchive( m_handle, path.c_str(), patchPathPrefix.c_str(), static_cast<DWORD>( flags ) );
        m_lastError = GetLastError();
        return false;
    }

    bool Archive::Impl::isPatchedArchive() const
    {
        assert( isOpened() );

        bool result = SFileIsPatchedArchive( m_handle );
        m_lastError = GetLastError();
        return false;
    }

    bool Archive::Impl::isFileExists( const path_t& fileName )
    {
        assert( isOpened() );

        bool result = SFileHasFile( m_handle, fileName.c_str() );
        m_lastError = GetLastError();

        if( m_lastError == ERROR_FILE_NOT_FOUND )
        {
            m_lastError = ERROR_SUCCESS;
        }

        return result;
    }

    File Archive::Impl::openFile( const path_t& fileName )
    {
        assert( isOpened() );
        return File( m_handle, fileName );
    }

    bool Archive::Impl::extractFile( const path_t& toExtract, const path_t& extracted )
    {
        bool result = SFileExtractFile( m_handle, toExtract.c_str(), extracted.c_str(), static_cast<DWORD>( StormLibCxxFileSearhFlags_FromMpq ) );
        m_lastError = GetLastError();
        return result;
    }

    Find Archive::Impl::findFiles( const std::string& mask, const path_t& listFile )
    {
        assert( isOpened() );
        return Find( m_handle, mask, listFile );
    }



    File::Impl::Impl( HANDLE archiveHandle, const path_t& fileName )
    {
        SFileOpenFileEx( archiveHandle, fileName.c_str(), static_cast<DWORD>( StormLibCxxFileSearhFlags_FromMpq ), &m_handle );
        m_lastError = GetLastError();
    }

    File::Impl::Impl( const path_t& fileName )
    {
        SFileOpenFileEx( nullptr, fileName.c_str(), static_cast<DWORD>( StormLibCxxFileSearhFlags_LocalFile ), &m_handle );
        m_lastError = GetLastError();
    }

    File::Impl::~Impl()
    {
        close();
    }

    HANDLE File::Impl::getHandle() const
    {
        return m_handle;
    }

    bool File::Impl::check() const
    {
        return m_handle != nullptr 
            && m_lastError == ERROR_SUCCESS;
    }

    bool File::Impl::isOpened() const
    {
        return m_handle != nullptr;
    }

    bool File::Impl::close()
    {
        if( m_handle )
        {
            bool result = SFileCloseFile( m_handle );
            m_lastError = GetLastError();
            m_handle = nullptr;
            return result;
        }
        else
        {
            return false;
        }
    }

    uint64_t File::Impl::getFileSize() const
    {
        assert( isOpened() );

        DWORD hi = 0;
		DWORD lo = SFileGetFileSize( m_handle, &hi );
        uint64_t fileSize = ( static_cast<uint64_t>( hi ) << 32 ) 
                            + static_cast<uint64_t>( lo );

        return fileSize;
    }


    Find::Impl::Impl( HANDLE archiveHandle, const std::string& mask, const path_t& listFile )
    {
        m_handle = SFileFindFirstFile( archiveHandle, mask.c_str(), &m_data, listFile.c_str() );
        m_lastError = GetLastError();
    }

    Find::Impl::~Impl()
    {
        close();
    }

    HANDLE Find::Impl::getHandle() const
    {
        return m_handle;
    }

    bool Find::Impl::check() const
    {
        return m_handle != nullptr 
            && m_lastError == ERROR_SUCCESS;
    }

    bool Find::Impl::isOpened() const
    {
        return m_handle != nullptr;
    }

    bool Find::Impl::close()
    {
        if( m_handle )
        {
            bool result = SFileFindClose( m_handle );
            m_lastError = GetLastError();
            m_handle = nullptr;
            return result;
        }
        else
        {
            return false;
        }
    }

    bool Find::Impl::moveForward()
    {
        assert( isOpened() );
        if( m_finished )
        {
            return false;
        }
        else
        {
            bool result = SFileFindNextFile( m_handle, &m_data );
            if( result == false )
            {
                m_finished = true;
            }
            return result;
        }
    }

    bool Find::Impl::isFinished() const
    {
        return m_finished;
    }

    std::string Find::Impl::getFileName() const
    {
        return std::string( m_data.cFileName );
    }

    std::string Find::Impl::getPlainName() const
    {
        return std::string( m_data.szPlainName );
    }

    size_t Find::Impl::getFileSize() const
    {
        return static_cast<size_t>( m_data.dwFileSize );
    }

    size_t Find::Impl::getFileFlags() const
    {
        return static_cast<size_t>( m_data.dwFileFlags );
    }

    size_t Find::Impl::getCompSize() const
    {
        return static_cast<size_t>( m_data.dwCompSize );
    }

    uint64_t Find::Impl::getFileTime() const
    {
        DWORD hi = m_data.dwFileTimeHi;
		DWORD lo = m_data.dwFileTimeLo;
        uint64_t fileTime = ( static_cast<uint64_t>( hi ) << 32 ) 
                            + static_cast<uint64_t>( lo );

        return fileTime;
    }

    LCID Find::Impl::getLocale() const
    {
        return static_cast<LCID>( m_data.lcLocale );
    }

}