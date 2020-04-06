//
// Created by ronyshchuk on 01.04.20.
//

#include <font.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Lexi {
FontName ParseXLFDName(const std::string& name)
{
    // 14 registers + first empty
    constexpr auto registersCount = 15;
    enum RegisterNames
    {
        FOUNDRY = 1,
        FAMILY_NAME,
        WEIGHT_NAME,
        SLANT,
        SET_WIDTH_NAME,
        ADD_STYLE_NAME,
        PIXEL_SIZE,
        POINT_SIZE,
        RESOLUTION_X,
        RESOLUTION_Y,
        SPACING,
        AVERAGE_WIDTH,
        CHARSET_REGISTRY,
        CHARSET_ENCODING
    };

    std::vector<std::string> xlfdRegisters;
    std::string registerField;

    std::istringstream tokenStream(name);
    while(std::getline(tokenStream, registerField, '-')) {
        xlfdRegisters.push_back(registerField);
    }

    if(!xlfdRegisters[0].empty() && xlfdRegisters.size() != registersCount) {
        std::cout << name << " is not a valid XLFD format" << std::endl;
        return {"", ""};
    }

    return FontName{xlfdRegisters[FAMILY_NAME], xlfdRegisters[ADD_STYLE_NAME]};
}
}  // namespace Lexi
