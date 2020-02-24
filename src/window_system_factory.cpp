//
// Created by romaonishuk on 01.12.19.
//

#include "window_system_factory.hpp"

// TODO: move this inside lexi_linux?!
#include "x_window_impl.hpp"

std::unique_ptr<Gui::WindowImpl> WindowSystemFactory::MakeWindowImpl(const GlyphParams params)
{
    return std::make_unique<Gui::XWindowImpl>(params);
}