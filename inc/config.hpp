//
// Created by ronyshchuk on 17.12.20.
//

#ifndef LEXI_CONFIG_HPP
#define LEXI_CONFIG_HPP

#include <string>

namespace Lexi
{
class Config
{
public:
    static Config& get() {
        static Config inst;
        return inst;
    }

    /**
     * @return Soft name.
     */
    [[nodiscard]] static constexpr auto GetName() {
        return softName;
    }

    /**
     * @return Information about soft, i.e name and version.
     */
    [[nodiscard]] std::string GetSoftInfo() const;

    /**
     * @return Location of Lexi-specific fonts.
     */
    [[nodiscard]] static std::string GetFontsPath();

private:
    Config() = default;
    ~Config() = default;

    static constexpr auto softName = "Lexi";
    float m_version = 1.100;
};
}

#endif  // LEXI_CONFIG_HPP
