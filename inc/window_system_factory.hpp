//
// Created by romaonishuk on 01.12.19.
//

#ifndef LEXI_WINDOW_SYSTEM_FACTORY_HPP
#define LEXI_WINDOW_SYSTEM_FACTORY_HPP

#include <memory>
#include <window_impl.hpp>

/**
 * @c WindowSystemFactory is a abstract factory that serves for creation of system specific
 * objects (like Windows).
 * @note Unlike the factory described in GoF, @c WindowSystemFactory decides which type of object
 * to create in compile time.
 */
class WindowSystemFactory
{
public:
    static WindowSystemFactory& Get()
    {
        static WindowSystemFactory self;
        return self;
    }

    static std::unique_ptr<Gui::WindowImpl> MakeWindowImpl(const GlyphParams& params);
    std::unique_ptr<Gui::WindowImpl> MakeChildWindowImpl(const GlyphParams& params, Gui::WindowImpl* windowImp);

private:
    WindowSystemFactory() = default;
    ~WindowSystemFactory() = default;
};

#endif  // LEXI_WINDOW_SYSTEM_FACTORY_HPP
