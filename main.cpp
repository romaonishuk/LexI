//
// Created by romaonishuk on 27.10.19.
//

#include <i_command.hpp>
#include "button.hpp"
#include "decorator.hpp"
#include "event_manager.hpp"
#include "font.hpp"
#include "i_glyph.hpp"
#include "menu.hpp"
#include "menu_item.hpp"
#include "page.hpp"
#include "scroller.hpp"
#include "text_label.hpp"
#include "text_view.hpp"
#include "window.hpp"

const auto Version = "Lexi v1.1";

#include <array>
const std::array supportedFontSizes = {
    "10", "10,5", "11", "12", "14", "16", "18", "20", "22", "32", "48", "54", "72", "96"};

int main()
{
    // Create window
    GlyphParams initial_window_params;
    initial_window_params.x = 0;
    initial_window_params.y = 0;
    initial_window_params.width = 1920;
    initial_window_params.height = 1080;

    Gui::Window window(initial_window_params);
    EventManager eventManager(&window);
    eventManager.addWindow(&window);

    Lexi::FontManager::Get().Init(&window);

    const auto defaultFont = "clean";  //"Ubuntu Mono";
    Lexi::FontManager::Get().SetFont(defaultFont);

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

    auto text_view = std::make_shared<TextView>(GlyphParams{360, 100, 1200, 800}, &window);
    window.Add(text_view);
    eventManager.addWindow(text_view.get());

    auto scroll_board = std::make_shared<Scroller>(GlyphParams{1895, 100, 20, 800}, text_view);
    window.Add(scroll_board);

    auto fontsMenu = std::make_shared<Gui::Menu>(GlyphParams{185, 5, 80, 40}, "Fonts", &window);
    top_panel->Add(fontsMenu);
    eventManager.addWindow(fontsMenu->getMenuWindow());

    auto fontSizeMenu = std::make_shared<Gui::DropDownMenu>(GlyphParams{365, 10, 80, 20}, "12", &window);
    top_panel->Add(fontSizeMenu);
    eventManager.addWindow(fontSizeMenu->getMenuWindow());

    auto bottom_panel =
        std::make_shared<BorderDecorator>(GlyphParams{0, 940, initial_window_params.width, 20}, Color::kBlack);
    auto status_line = std::make_shared<TextLabel>(GlyphParams{10, 955, 100, 20}, Version);
    bottom_panel->Add(status_line);
    window.Add(bottom_panel);

    // Actions
    openMenuItem->SetOnFocusedAction([&] {
        bottom_panel->ClearGlyph(&window);
        status_line->ChangeText("Open file", &window);
        bottom_panel->Draw(&window);
    });

    quitMenuItem->SetCommand(std::make_unique<QuitCommand>(&eventManager));
    quitMenuItem->SetOnFocusedAction([&] {
        bottom_panel->ClearGlyph(&window);
        status_line->ChangeText("Close application", &window);
        bottom_panel->Draw(&window);
    });

    for(const auto& it: Lexi::FontManager::Get().GetFontList()) {
        auto menuItem = std::make_shared<Gui::MenuItem>(it);
        fontsMenu->Add(menuItem);
        menuItem->SetOnFocusedAction([&, it] {
            bottom_panel->ClearGlyph(&window);
            status_line->ChangeText(it, &window);
            bottom_panel->Draw(&window);
        });
        menuItem->SetOnButtonPressedAction([&, it] { Lexi::FontManager::Get().SetFont(it); });
    }

    for(const auto& it: supportedFontSizes) {
        auto item = std::make_shared<Gui::MenuItem>(it);
        item->SetOnButtonPressedAction([it] { Lexi::FontManager::Get().SetFontSize(it); });
        fontSizeMenu->Add(std::move(item));
    }

    text_view->SetOnPageAddedAction([&]() {
        scroll_board->UpdateScaling();
        scroll_board->UpdateScrollerPosition(&window);
    });

    // TODO(rmn): think about textView/ScrollBar dependency!
    text_view->SetOnVisibleAreaUpdateAction([&] { scroll_board->UpdateScrollerPosition(&window); });
    text_view->SetOnScrollAction([&](const Lexi::Event& event) { scroll_board->ProcessEvent(&window, event); });

    eventManager.RunLoop();
}
