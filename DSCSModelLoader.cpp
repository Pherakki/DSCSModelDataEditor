#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
#include <FileFormats/DSCS/NameFile/NameReadWrite.hpp>
#include <FileFormats/DSCS/SkelFile/SkelReadWrite.hpp>
#include <FileFormats/DSCS/GeomFile/GeomReadWrite.hpp>

int main(int arc, char** argv)
{
    
    std::string name_path = argv[1];
    std::string name_path_out = ".\\out";
    bool path_exists = std::filesystem::exists(name_path);
    if (path_exists)
        std::cout << "Path exists" << std::endl;
    else
    {
        std::cout << "Path does not exist" << std::endl;
        return 1;
    }

    // Name Test
    FileFormats::DSCS::NameFile::NameReadWrite nameFile;
    nameFile.read(name_path + ".name");
    nameFile.write(name_path_out + ".name");

    // Skel Test
    FileFormats::DSCS::SkelFile::SkelReadWrite skelFile;
    skelFile.read(name_path + ".skel");
    skelFile.write(name_path_out + ".skel");

    // Geom Test
    FileFormats::DSCS::GeomFile::GeomReadWrite geomFile;
    geomFile.read(name_path + ".geom");
    geomFile.write(name_path_out + ".geom");

    return 0;
}
