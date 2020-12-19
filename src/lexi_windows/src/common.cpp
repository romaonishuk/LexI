//
// Created by ronyshchuk on 19.12.2020.
//

#include "common.hpp"
#include <windows.h>
#include <winbase.h>
#include <iostream>
namespace Lexi {
[[nodiscard]] std::string GetCurrentDir()
{
    char buffer[256];
    if (::GetCurrentDirectory(sizeof(buffer), buffer) != 0) {
        return buffer;
    }

    std::cout << "Failed to get current dir!" << std::endl;
    return {};
}

[[nodiscard]] bool IsDir(const std::string& path)
{
    auto attr = GetFileAttributesA(path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    return attr & FILE_ATTRIBUTE_DIRECTORY;
}
}
