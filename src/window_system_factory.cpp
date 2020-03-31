//
// Created by romaonishuk on 01.12.19.
//

#include "window_system_factory.hpp"
#include <window.hpp>

// TODO: move this inside lexi_linux?!
#include "x_window_impl.hpp"

std::unique_ptr<Gui::WindowImpl> WindowSystemFactory::MakeWindowImpl(const GlyphParams& params)
{
    return std::make_unique<Gui::XWindowImpl>(params);
}

// TODO(rmn): still tempo solution
std::unique_ptr<Gui::WindowImpl> WindowSystemFactory::MakeChildWindowImpl(
    const GlyphParams& params, Gui::WindowImpl* windowImpl)
{
    auto p = dynamic_cast<Gui::XWindowImpl*>(windowImpl);
    auto ptr = std::make_unique<Gui::XWindowImpl>(params, p);
    return ptr;
}
