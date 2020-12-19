//
// Created by romaonishuk on 19.12.2020.
//

#ifndef LEXI_WINDOWS_HELPERS_H
#define LEXI_WINDOWS_HELPERS_H

#include <Windows.h>

namespace Lexi::Windows
{
std::string GetLastErrorAsString()
{
    auto errorMessageID = GetLastError();
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);

    std::string message(messageBuffer, size);

    LocalFree(messageBuffer);
    return message;
}
}

#endif //LEXI_WINDOWS_HELPERS_H
