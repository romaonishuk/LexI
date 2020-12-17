//
// Created by ronyshchuk on 01.04.20.
//

#ifndef LEXI_COMMON_HPP
#define LEXI_COMMON_HPP

#include <string>

namespace Lexi {
struct FontName;

FontName ParseXLFDName(const std::string& name);

/**
 * @note Cross-platform getter, since std::filesystem requires new compilers, which I don't want to rely on.
 * @return Path to current dir.
 */
[[nodiscard]] std::string GetCurrentDir();

/**
 * Check whether @p path is directory.
 * @param path Path to checked entity.
 * @return True if @p path exists and it's dir.
 */
[[nodiscard]] bool IsDir(const std::string& path);

}  // namespace Lexi

#endif  // LEXI_COMMON_HPP
