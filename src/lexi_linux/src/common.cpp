//
// Created by ronyshchuk on 17.12.20.
//

#include "common.hpp"
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

namespace Lexi {
[[nodiscard]] std::string GetCurrentDir()
{
    char buffer[256];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return buffer;
    }

    std::cout << "Failed to get current dir!" << std::endl;
    return {};
}

[[nodiscard]] bool IsDir(const std::string& path)
{
    struct stat info{};
    if(stat(path.c_str() , &info) != 0) {
        return false;
    }
    return info.st_mode & S_IFDIR;
}
}
