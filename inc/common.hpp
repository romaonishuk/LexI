//
// Created by ronyshchuk on 01.04.20.
//

#ifndef LEXI_COMMON_HPP
#define LEXI_COMMON_HPP

#include <string>

namespace Lexi {
struct FontName;

FontName ParseXLFDName(const std::string& name);
}  // namespace Lexi
#endif  // LEXI_COMMON_HPP
