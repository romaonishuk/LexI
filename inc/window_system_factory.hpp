//
// Created by romaonishuk on 01.12.19.
//

#ifndef LEXI_WINDOW_SYSTEM_FACTORY_HPP
#define LEXI_WINDOW_SYSTEM_FACTORY_HPP

#include <memory>
#include <window_impl.hpp>

class WindowSystemFactory
{
public:
    static WindowSystemFactory& Get()
    {
        static WindowSystemFactory self;
        return self;
    }

    std::unique_ptr<Gui::WindowImpl> MakeWindowImpl(const GlyphParams& params);
    std::unique_ptr<Gui::WindowImpl> MakeChildWindowImpl(const GlyphParams& params, Gui::WindowImpl *windowImp);
private:
    WindowSystemFactory() = default;
    ~WindowSystemFactory() = default;
};

#endif //LEXI_WINDOW_SYSTEM_FACTORY_HPP
