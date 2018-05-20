// StormLibCxx (c) 2018 YarikTH
// This code is licensed under MIT license (see LICENSE for details)

#include "StormLibCxxImpl.h"

namespace StormLibCxx
{
    Archive::Impl::Impl()
    {
    }

    Archive::Impl::Impl( const path_t& path )
    {
        open( path );
    }

    Archive::Impl::~Impl()
    {
        close();
    }

    HANDLE Archive::Impl::getHandle() const
    {
        return m_handle;
    }

    Archive::Impl::operator bool() const 
    { 
        return m_handle != nullptr; 
    }

    bool Archive::Impl::check() const
    {
        return m_handle != nullptr 
            && m_lastError == ERROR_SUCCESS;
    }

    bool Archive::Impl::open( const path_t& path )
    {
        assert( !isOpened() );

        // @todo handle args
        // DWORD dwPriority, DWORD dwFlags

        m_lastError = ERROR_SUCCESS;
        bool result = SFileOpenArchive( path.c_str(), 0, MPQ_OPEN_READ_ONLY, &m_handle );
        m_lastError = GetLastError();
        return result;
    }

    bool Archive::Impl::create( const path_t& path )
    {
        assert( !isOpened() );

        // @todo handle args
        // DWORD dwCreateFlags, DWORD dwMaxFileCount

        m_lastError = ERROR_SUCCESS;
        bool result = SFileCreateArchive( path.c_str(), 0, 0, &m_handle );
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

    bool Archive::Impl::isFileExists( const path_t& path )
    {
        if( !m_handle )
            return false;

        bool result = SFileHasFile( m_handle, path.c_str() );
        m_lastError = GetLastError();

        if( m_lastError == ERROR_FILE_NOT_FOUND )
        {
            m_lastError = ERROR_SUCCESS;
        }

        return result;
    }
}