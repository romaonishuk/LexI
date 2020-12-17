//
// Created by ronyshchuk on 17.12.20.
//

#include "config.hpp"
#include "common.hpp"
#include <sstream>

//#include <filesystem>

namespace Lexi
{
[[nodiscard]] std::string Config::GetSoftInfo() const
{
    auto cutVersion = [&](float version) {
      std::stringstream out;
      out.precision(3);
      out << std::fixed << version;
      return out.str();
    };

    std::string softInfo = GetName();
    softInfo += " v" + cutVersion(m_version);
    return softInfo;
}

std::string Config::GetFontsPath()
{
    std::string fontsLocation;
    #ifndef NDEBUG
        // TODO(rmn): fonts should be installed in some location like /etc/Lexi/fonts in future
        auto currentDir = GetCurrentDir();
        if(!currentDir.empty()) {
            fontsLocation = currentDir + "/resources/fonts";

            if(!IsDir(fontsLocation)) {
                fontsLocation.clear();
            }
        }
    #endif

    return fontsLocation;
}
}
