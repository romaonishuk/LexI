
#include <iostream>

#include "window.hpp"
#include "glyphs.hpp"
#include "event_manager.hpp"
#include "button.hpp"
#include "text_view.hpp"
#include "decorator.hpp"

int main()
{
    // Create window

    GlyphParams initial_window_params;
    initial_window_params.x = 0;
    initial_window_params.y = 0;
    initial_window_params.width = 1024;
    initial_window_params.height= 1024;
    
    Gui::Window window(initial_window_params);

    
    auto text_view = std::make_shared<TextView>(GlyphParams{5, 5, 1020, 800});
    window.Add(std::make_shared<BorderDecorator>(GlyphParams{5, 5, 1020, 800}, text_view));

    window.Add(std::make_shared<Button>(GlyphParams{1, 900, 80, 40}, "Open"));

    EventManager eventManager(&window);
    eventManager.RunLoop();
}