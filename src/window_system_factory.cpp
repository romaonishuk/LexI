//
// Created by romaonishuk on 01.12.19.
//

#include "window_system_factory.hpp"
#include <window.hpp>

#if defined  __linux__
#include "x_window_impl.hpp"
#elif defined _WIN64
#include "w_window_impl.hpp"
#endif

std::unique_ptr<Gui::WindowImpl> WindowSystemFactory::MakeWindowImpl(const GlyphParams& params)
{
#if defined  __linux__
    return std::make_unique<Gui::XWindowImpl>(params);
#else
    return std::make_unique<Gui::WWindowImpl>(params);
#endif
}

// TODO(rmn): still tempo solution
std::unique_ptr<Gui::WindowImpl> WindowSystemFactory::MakeChildWindowImpl(
    const GlyphParams& params, Gui::WindowImpl* windowImpl)
{
#if defined  __linux__
    return std::make_unique<Gui::XWindowImpl>(params, dynamic_cast<Gui::XWindowImpl*>(windowImpl));
#else
    return std::make_unique<Gui::WWindowImpl>(params);
#endif
}
