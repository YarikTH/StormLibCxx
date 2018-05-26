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

void testFileExtraction( const std::string& path, const std::string& filepath, const std::string& extracted )
{
    Archive ar( path );
    if( ar )
    {
        if( ar.extractFile( filepath, extracted ) )
        {
            std::cout << filepath << " is extracted" << std::endl;
        }
        else
        {
            std::cout << filepath << " is not extracted" << std::endl;
        }
    }
    else
    {
        std::cout << "Can't open archive " << path << std::endl;
    }
}

void testFileFind( const std::string& path, const std::string& mask, const std::string& listfile )
{
    Archive ar( path );
    if( ar )
    {
        Find find = ar.findFiles( mask, listfile );
        if( !find )
        {
            std::cout << "Can't find" << mask << " in " << path << std::endl;
            return;
        }

        for( ; !find.isFinished(); ++find )
        {
            std::cout << find.getFileName() << std::endl;
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

    testFileExtraction( "data/DIABDAT.MPQ", "ui_art\\title.pcx", "temp/title.pcx" );

    testFileFind( "data/DIABDAT.MPQ", "ui_art\\*", "data/Diablo I.txt" );

	int i;
	std::cin >> i;

    return 0;
}