#include "DragAndDrop.h"

#include <imgui.h>
#include <portable-file-dialogs.h>

namespace gui {

    std::optional<std::filesystem::path> renderDropZone()
    {
        std::optional<std::filesystem::path> chosenFile;

        ImGui::BeginChild("DropZone", ImVec2(-1.0f, 140.0f), ImGuiChildFlags_Borders);
        ImGui::TextDisabled("Przeci¹gnij i upuœæ plik .osr gdziekolwiek w oknie");
        ImGui::Spacing();

        if (ImGui::Button("Wybierz plik .osr...", ImVec2(180.0f, 36.0f))) {
            auto selection = pfd::open_file(
                "Wybierz powtórkê osu!",
                "",
                { "Pliki powtórek osu! (*.osr)", "*.osr" }
            ).result();

            if (!selection.empty()) {
                chosenFile = std::filesystem::path(
                    reinterpret_cast<const char8_t*>(selection[0].c_str())
                );
            }
        }
        ImGui::EndChild();

        return chosenFile;
    }

}