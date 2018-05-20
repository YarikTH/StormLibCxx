// StormLibCxx (c) 2018 YarikTH
// This code is licensed under MIT license (see LICENSE for details)
#pragma once

#include <string>
#include <memory>

namespace StormLibCxx
{
    std::string getVersionString();

    using HANDLE = void*;
    using path_t = std::string;

    class File;

    class Archive
    {
    public:
        Archive();

        Archive( const path_t& path );

        Archive( const Archive& ) = delete;
        Archive& operator=( const Archive& ) = delete;

        Archive( Archive&& );

        ~Archive();

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

        /// @todo add openFile

    private:
        class Impl;

        std::unique_ptr<Impl> m_pimpl;
    };

    class File
    {

    };
}