// StormLibCxx (c) 2018 YarikTH
// This code is licensed under MIT license (see LICENSE for details)
#include "StormLibCxx/StormLibCxx.h"
#include "StormLibCxxImpl.h"

namespace StormLibCxx
{
    std::string getVersionString()
    {
        return STORMLIB_VERSION_STRING;
    }

    Archive::Archive()
        : m_pimpl( new Impl )
    {
    }

    Archive::Archive( const path_t& path )
        : m_pimpl( new Impl( path ) )
    {
    }

    Archive::Archive( Archive&& rhs )
    {
        m_pimpl = std::move( rhs.m_pimpl );
    }

    Archive::~Archive()
    {
    }

    HANDLE Archive::getHandle() const
    {
        return m_pimpl ? m_pimpl->getHandle() : HANDLE();
    }

    Archive::operator bool() const 
    { 
        return getHandle() != nullptr; 
    }

    bool Archive::check() const
    {
        return m_pimpl->check();
    }

    bool Archive::open( const path_t& path )
    {
        return m_pimpl->open( path );
    }

    bool Archive::create( const path_t& path )
    {
        return m_pimpl->create( path );
    }

    bool Archive::isOpened() const
    {
        return m_pimpl->isOpened();
    }

    bool Archive::close()
    {
        return m_pimpl->close();
    }

    bool Archive::flush()
    {
        return m_pimpl->flush();
    }

    size_t Archive::getMaxFileCount() const
    {
        return m_pimpl->getMaxFileCount();
    }

    bool Archive::setMaxFileCount( size_t value )
    {
        return m_pimpl->setMaxFileCount( value );
    }

    bool Archive::isFileExists( const path_t& path)
    {
        return m_pimpl->isFileExists( path );
    }
}