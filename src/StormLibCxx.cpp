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

    Archive::Archive( const path_t& path, StormLibCxxArhiveOpenFlags flags )
        : m_pimpl( new Impl( path, flags ) )
    {
    }

    Archive::Archive( Archive&& rhs )
        : m_pimpl( std::move( rhs.m_pimpl ) )
    {
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

    bool Archive::open( const path_t& path, StormLibCxxArhiveOpenFlags flags )
    {
        return m_pimpl->open( path, flags );
    }

    bool Archive::create( const path_t& path, size_t maxFilesCount, StormLibCxxArhiveCreateFlags flags )
    {
        return m_pimpl->create( path, maxFilesCount, flags );
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

    bool Archive::addListFile( const path_t& path )
    {
        return m_pimpl->addListFile( path );
    }

    size_t Archive::getMaxFileCount() const
    {
        return m_pimpl->getMaxFileCount();
    }

    bool Archive::setMaxFileCount( size_t value )
    {
        return m_pimpl->setMaxFileCount( value );
    }

    StormLibCxxArhiveAttributesFlags Archive::getAttributes() const
    {
        return m_pimpl->getAttributes();
    }

    bool Archive::setAttributes( StormLibCxxArhiveAttributesFlags value )
    {
        return m_pimpl->setAttributes( value );
    }

    bool Archive::updateFileAttributes( const path_t& path )
    {
        return m_pimpl->updateFileAttributes( path );
    }

    bool Archive::openPatchArchive( const path_t& path, const path_t& patchPathPrefix, StormLibCxxArhiveOpenFlags flags )
    {
        return m_pimpl->openPatchArchive( path, patchPathPrefix, flags );
    }

    bool Archive::isPatchedArchive() const
    {
        return m_pimpl->isPatchedArchive();
    }

    bool Archive::isFileExists( const path_t& fileName )
    {
        return m_pimpl->isFileExists( fileName );
    }

    File Archive::openFile( const path_t& fileName )
    {
        return m_pimpl->openFile( fileName );
    }

    bool Archive::extractFile( const path_t& toExtract, const path_t& extracted )
    {
        return m_pimpl->extractFile( toExtract, extracted );
    }

    Find Archive::findFiles( const std::string& mask, const path_t& listFile )
    {
        return m_pimpl->findFiles( mask, listFile );
    }




    File::File( HANDLE archiveHandle, const path_t& fileName )
        : m_pimpl( new Impl( archiveHandle, fileName ) )
    {

    }

    File::File( const path_t& fileName )
        : m_pimpl( new Impl( fileName ) )
    {

    }

    File::File( File&& rhs )
        : m_pimpl( std::move( rhs.m_pimpl ) )
    {
    }

    File::~File()
    {
    }

    HANDLE File::getHandle() const
    {
        return m_pimpl ? m_pimpl->getHandle() : HANDLE();
    }

    File::operator bool() const
    {
        return getHandle() != nullptr;
    }

    bool File::check() const
    {
        return m_pimpl->check();
    }

    bool File::isOpened() const
    {
        return m_pimpl->isOpened();
    }

    bool File::close()
    {
        return m_pimpl->close();
    }

    uint64_t File::getFileSize() const
    {
        return m_pimpl->getFileSize();
    }





    Find::Find( HANDLE archiveHandle, const std::string& mask, const path_t& listFile )
        : m_pimpl( new Impl( archiveHandle, mask, listFile ) )
    {
    }

    Find::Find( Find&& rhs )
        : m_pimpl( std::move( rhs.m_pimpl ) )
    {
    }

    Find::~Find()
    {
    }

    HANDLE Find::getHandle() const
    {
        return m_pimpl ? m_pimpl->getHandle() : HANDLE();
    }

    Find::operator bool() const
    {
        return getHandle() != nullptr;
    }

    bool Find::check() const
    {
        return m_pimpl->check();
    }

    bool Find::isOpened() const
    {
        return m_pimpl->isOpened();
    }

    bool Find::close()
    {
        return m_pimpl->close();
    }

    bool Find::moveForward()
    {
        return m_pimpl->moveForward();
    }

    Find& Find::operator++()
    {
        m_pimpl->moveForward();
        return *this;
    }

    bool Find::isFinished() const
    {
        return m_pimpl->isFinished();
    }

    std::string Find::getFileName() const
    {
        return m_pimpl->getFileName();
    }

    std::string Find::getPlainName() const
    {
        return m_pimpl->getPlainName();
    }

    size_t Find::getFileSize() const
    {
        return m_pimpl->getFileSize();
    }

    size_t Find::getFileFlags() const
    {
        return m_pimpl->getFileFlags();
    }

    size_t Find::getCompSize() const
    {
        return m_pimpl->getCompSize();
    }

    uint64_t Find::getFileTime() const
    {
        return m_pimpl->getFileTime();
    }

    LCID Find::getLocale() const
    {
        return m_pimpl->getLocale();
    }

}