//
// Created by romaonishuk on 27.10.19.
//

#include <i_command.hpp>
#include "button.hpp"
#include "decorator.hpp"
#include "event_manager.hpp"
#include "i_glyph.hpp"
#include "menu.hpp"
#include "menu_item.hpp"
#include "text_label.hpp"
#include "text_view.hpp"
#include "window.hpp"

const auto Version = "Lexi v1.0";

int main()
{
    // Create window
    GlyphParams initial_window_params;
    initial_window_params.x = 0;
    initial_window_params.y = 0;
    initial_window_params.width = 800;
    initial_window_params.height = 1024;

    Gui::Window window(initial_window_params);
    EventManager eventManager(&window);
    eventManager.addWindow(&window);

    auto top_panel =
        std::make_shared<BorderDecorator>(GlyphParams{0, 0, initial_window_params.width, 50}, Color::kWhite);
    window.Add(top_panel);

    auto fileMenu = std::make_shared<Gui::Menu>(GlyphParams{5, 5, 80, 40}, "File", &window);
    auto openMenuItem = std::make_shared<Gui::MenuItem>("Open");
    auto quitMenuItem = std::make_shared<Gui::MenuItem>("Quit");
    fileMenu->Add(openMenuItem);
    fileMenu->Add(quitMenuItem);
    top_panel->Add(fileMenu);
    eventManager.addWindow(fileMenu->getMenuWindow());

    top_panel->Add(std::make_shared<Button>(GlyphParams{95, 5, 80, 40}, "Edit"));
    top_panel->Add(std::make_shared<Button>(GlyphParams{275, 5, 80, 40}, "Symbol"));

    auto fontsMenu = std::make_shared<Gui::Menu>(GlyphParams{185, 5, 80, 40}, "Fonts", &window);
    top_panel->Add(fontsMenu);
    eventManager.addWindow(fontsMenu->getMenuWindow());

    auto text_view_border = std::make_shared<BorderDecorator>(GlyphParams{40, 100, 700, 800}, Color::kBlack);
    text_view_border->Add(std::make_shared<TextView>(GlyphParams{40, 100, 700, 800}));
    window.Add(text_view_border);

    auto scroll_board = std::make_shared<BorderDecorator>(GlyphParams{750, 100, 20, 800}, Color::kBlack);
    window.Add(scroll_board);

    auto bottom_panel =
        std::make_shared<BorderDecorator>(GlyphParams{0, 1000, initial_window_params.width, 20}, Color::kBlack);
    auto status_line = std::make_shared<TextLabel>(GlyphParams{10, 1015, 100, 20}, Version);
    bottom_panel->Add(status_line);
    window.Add(bottom_panel);

    // Actions
    openMenuItem->SetOnFocusedAction([&] {
        bottom_panel->ClearGlyph(&window);
        status_line->ChangeText("Open file", &window);
    });

    quitMenuItem->SetCommand(std::make_unique<QuitCommand>(&eventManager));
    quitMenuItem->SetOnFocusedAction([&] {
        bottom_panel->ClearGlyph(&window);
        status_line->ChangeText("Close application", &window);
    });

    eventManager.RunLoop();
}