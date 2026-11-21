#include "DragAndDrop.h"
#include "src/GUI/Theme/Theme.h"

#include <imgui.h>
#include <portable-file-dialogs.h>

namespace gui {

    std::optional<std::filesystem::path> renderDropZone(
        const std::optional<std::filesystem::path>& currentFile,
        const std::string& errorMessage)
    {
        std::optional<std::filesystem::path> chosenFile;

        ImGuiChildFlags childFlags = ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY;
        ImGui::BeginChild("DropZone", ImVec2(-1.0f, 0.0f), childFlags);

        ImGui::TextDisabled("Drop file .osr");
        ImGui::Spacing();

        if (ImGui::Button("Choose file .osr", ImVec2(170.0f, 34.0f))) {
            auto selection = pfd::open_file(
                "Choose replay!",
                "",
                { "File: (*.osr)", "*.osr" }
            ).result();

            if (!selection.empty()) {
                chosenFile = std::filesystem::path(
                    reinterpret_cast<const char8_t*>(selection[0].c_str())
                );
            }
        }

        ImGui::Spacing();

        if (!errorMessage.empty()) {
            ImGui::TextColored(colors::Error, "%s", errorMessage.c_str());
        }
        else if (currentFile.has_value()) {
            auto u8Name = currentFile->filename().u8string();
            std::string fileName(reinterpret_cast<const char*>(u8Name.c_str()));

            if (BoldFont != nullptr) {
                ImGui::PushFont(BoldFont);
            }
            ImGui::TextWrapped("%s", fileName.c_str());
            if (BoldFont != nullptr) {
                ImGui::PopFont();
            }
        }
        else {
            ImGui::TextDisabled("No file yet");
        }

        ImGui::EndChild();

        return chosenFile;
    }

}