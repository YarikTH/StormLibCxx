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

        Impl( const path_t& path );

        Impl( const Impl& ) = delete;
        Impl& operator=( const Impl& ) = delete;

        ~Impl();

        HANDLE getHandle() const;

        explicit operator bool() const;

        bool check() const;

        bool open( const path_t& path );

        bool create( const path_t& path );

        bool isOpened() const;

        bool close();

        bool flush();

        size_t getMaxFileCount() const;

        bool setMaxFileCount( size_t value );

        bool isFileExists( const path_t& path );

    private:
        HANDLE m_handle = nullptr;
        mutable DWORD m_lastError = ERROR_SUCCESS;
    };

}