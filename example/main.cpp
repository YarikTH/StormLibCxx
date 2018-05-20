#include "StormLibCxx/StormLibCxx.h"
#include <iostream>

using namespace StormLibCxx;

void checkArchive( const std::string& path )
{
    Archive ar( path );
    if( ar )
    {
        std::cout << path << " was opened successfully" << std::endl;
        std::cout << "Max files count = " << ar.getMaxFileCount() << std::endl;
    }
    else
    {
        std::cout << "Can't open " << path << std::endl;
    }
}

void checkFileInArchive( const std::string& path, const std::string& filepath )
{
    Archive ar( path );
    if( ar )
    {
        if( ar.isFileExists( filepath ) )
        {
            std::cout << filepath << " is exists" << std::endl;
        }
        else
        {
            std::cout << filepath << " is not exists" << std::endl;
        }
    }
    else
    {
        std::cout << "Can't open archive " << path << std::endl;
    }
}

int main(int argc, char *argv[])
{
	std::cout << "Test StormLibCxx v" << StormLibCxx::getVersionString() << std::endl;

    checkArchive( "data/DIABDAT.MPQ" );
    checkArchive( "data/DIABDAT2.MPQ" );

    checkFileInArchive( "data/DIABDAT.MPQ", "ui_art\\title.pcx" );
    checkFileInArchive( "data/DIABDAT.MPQ", "wtf" );

	int i;
	std::cin >> i;

    return 0;
}