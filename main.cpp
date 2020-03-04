
#include <iostream>

#include "window.hpp"
#include "i_glyph.hpp"
#include "event_manager.hpp"
#include "button.hpp"
#include "text_view.hpp"
#include "decorator.hpp"
#include "text_label.hpp"

int main()
{
    // Create window

    GlyphParams initial_window_params;
    initial_window_params.x = 0;
    initial_window_params.y = 0;
    initial_window_params.width = 800;
    initial_window_params.height= 1024;
    
    Gui::Window window(initial_window_params);

    auto top_panel = std::make_shared<BorderDecorator>(GlyphParams{0, 0, initial_window_params.width, 50}, Color::kWhite);
    top_panel->Add(std::make_shared<Button>(GlyphParams{5, 5, 80, 40}, "File"));
    top_panel->Add(std::make_shared<Button>(GlyphParams{95, 5, 80, 40}, "Edit"));
    top_panel->Add(std::make_shared<Button>(GlyphParams{185, 5, 80, 40}, "Style"));
    top_panel->Add(std::make_shared<Button>(GlyphParams{275, 5, 80, 40}, "Symbol"));
    window.Add(top_panel);

    // TODO(rmn): fix with C++17
    auto text_view = std::make_shared<TextView>(GlyphParams{40, 100, 700, 800});
    auto text_view_border = std::make_shared<BorderDecorator>(GlyphParams{40, 100, 700, 800}, Color::kBlack);
    text_view_border->Add(text_view);
    window.Add(text_view_border);

    auto scroll_board = std::make_shared<BorderDecorator>(GlyphParams{750, 100, 20, 800}, Color::kBlack);
    window.Add(scroll_board);

    auto bottom_panel = std::make_shared<BorderDecorator>(GlyphParams{0, 1000, initial_window_params.width, 20}, Color::kBlack);
    bottom_panel->Add(std::make_shared<TextLabel>(GlyphParams{100, 1010, 100, 20}, "This is status line!"));
    window.Add(bottom_panel);

    EventManager eventManager(&window);
    eventManager.RunLoop();
}