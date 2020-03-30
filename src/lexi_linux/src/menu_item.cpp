//
// Created by ronyshchuk on 30.03.20.
//

#include "window.hpp"
#include "menu_item.hpp"
#include "i_command.hpp"

namespace Gui {
    void MenuItem::SetCommand(std::unique_ptr<ICommand> cmd) {
        m_command = std::move(cmd);
    }

    void MenuItem::Draw(Window *w) {
        w->SetForeground(Color::kWhite);

        // top
        w->DrawLine({m_params.x, m_params.y}, {m_params.x + m_params.width, m_params.y});
        // left
        w->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});

        w->SetForeground(Color::kBlack);

        // bottom
        w->DrawLine({m_params.x, m_params.y + m_params.height},
                    {m_params.x + m_params.width, m_params.y + m_params.height});

        // right
        w->DrawLine({m_params.x + m_params.width, m_params.y},
                    {m_params.x + m_params.width, m_params.y + m_params.height});


        // TODO(rmn): font should be included
        constexpr auto hMagic = 5;
        constexpr auto wMagic = 10;

        w->DrawText({m_params.x + m_params.width / 2 - wMagic, m_params.y + m_params.height / 2 + hMagic}, m_text);
    }

    void MenuItem::ProcessEvent(Window *w, const Point &p, const EventType &ev) {
        switch (ev) {
            case EventType::ButtonPressed:
                w->SetForeground(Color::kWhite);

                // bottom
                w->DrawLine({m_params.x, m_params.y + m_params.height},
                            {m_params.x + m_params.width, m_params.y + m_params.height});
                // right
                w->DrawLine({m_params.x + m_params.width, m_params.y},
                            {m_params.x + m_params.width, m_params.y + m_params.height});

                w->SetForeground(Color::kBlack);

                // top
                w->DrawLine({m_params.x, m_params.y}, {m_params.x + m_params.width, m_params.y});
                // left
                w->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});

                w->SetForeground(Color::kGray);
                break;
            case EventType::ButtonReleased:
                Draw(w);
                if (m_command) {
                    m_command->Execute();
                }
                break;
            case EventType::FocusedIn:
                w->ReDraw();
                w->FillRectangle(m_params, m_params.width, m_params.height, kLightBlue);

                w->SetForeground(Color::kBlack);
                w->DrawText({m_params.x + m_params.width / 2 - 10, m_params.y + m_params.height / 2 + 5}, m_text);
                break;
            default:
                return;
        }
    }
}