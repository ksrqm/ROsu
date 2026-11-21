#include "Theme.h"

#include <filesystem>

namespace gui {

    namespace {

        void loadFonts()
        {
            ImGuiIO& io = ImGui::GetIO();
            const std::filesystem::path regularPath = "C:/Windows/Fonts/segoeui.ttf";
            const std::filesystem::path boldPath = "C:/Windows/Fonts/segoeuib.ttf";

            if (std::filesystem::exists(regularPath)) {
                static ImVector<ImWchar> glyphRanges;
                ImFontGlyphRangesBuilder builder;
                builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
                builder.AddText("¥ÆÊ£ÑÓŒ¯¹æê³ñóœŸ¿");
                builder.BuildRanges(&glyphRanges);

                // Pierwsza dodana czcionka staje siê domyœln¹ w ca³ym ImGui
                RegularFont = io.Fonts->AddFontFromFileTTF(
                    regularPath.string().c_str(), 17.0f, nullptr, glyphRanges.Data
                );

                if (std::filesystem::exists(boldPath)) {
                    BoldFont = io.Fonts->AddFontFromFileTTF(
                        boldPath.string().c_str(), 17.0f, nullptr, glyphRanges.Data
                    );
                }
            }
            else {
                RegularFont = io.Fonts->AddFontDefault();
            }
        }

        void applyStyleMetrics(ImGuiStyle& style)
        {
            // Wiêcej oddechu wewn¹trz okien i kontrolek
            style.WindowPadding = ImVec2(16.0f, 16.0f);
            style.FramePadding = ImVec2(12.0f, 7.0f);
            style.ItemSpacing = ImVec2(10.0f, 10.0f);
            style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);

            // Zaokr¹glenia krawêdzi
            style.WindowRounding = 8.0f;
            style.ChildRounding = 8.0f;
            style.FrameRounding = 6.0f;
            style.PopupRounding = 6.0f;
            style.ScrollbarRounding = 6.0f;
            style.GrabRounding = 6.0f;
            style.TabRounding = 6.0f;

            // Subtelne obramowania
            style.WindowBorderSize = 0.0f;
            style.ChildBorderSize = 1.0f;
            style.FrameBorderSize = 0.0f;
            style.ScrollbarSize = 12.0f;
        }

        void applyColorPalette(ImGuiStyle& style)
        {
            ImVec4* c = style.Colors;

            c[ImGuiCol_Text] = colors::TextPrimary;
            c[ImGuiCol_TextDisabled] = colors::TextMuted;

            c[ImGuiCol_WindowBg] = colors::Background;
            c[ImGuiCol_ChildBg] = colors::Surface;
            c[ImGuiCol_PopupBg] = colors::Surface;

            c[ImGuiCol_Border] = colors::Border;
            c[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

            // Pola tekstowe, listy, checkboxy
            c[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.19f, 1.00f);
            c[ImGuiCol_FrameBgHovered] = colors::SurfaceHover;
            c[ImGuiCol_FrameBgActive] = ImVec4(0.21f, 0.21f, 0.26f, 1.00f);

            // Przyciski w kolorze akcentu
            c[ImGuiCol_Button] = colors::Accent;
            c[ImGuiCol_ButtonHovered] = colors::AccentHover;
            c[ImGuiCol_ButtonActive] = colors::AccentActive;

            // Suwaki i zaznaczenia
            c[ImGuiCol_CheckMark] = colors::Accent;
            c[ImGuiCol_SliderGrab] = colors::Accent;
            c[ImGuiCol_SliderGrabActive] = colors::AccentHover;

            // Nag³ówki i zak³adki
            c[ImGuiCol_Header] = ImVec4(1.00f, 0.40f, 0.67f, 0.22f);
            c[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.40f, 0.67f, 0.35f);
            c[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.40f, 0.67f, 0.48f);

            c[ImGuiCol_Separator] = colors::Border;
            c[ImGuiCol_ScrollbarBg] = colors::Background;
            c[ImGuiCol_ScrollbarGrab] = colors::Border;
            c[ImGuiCol_ScrollbarGrabHovered] = colors::TextMuted;
            c[ImGuiCol_ScrollbarGrabActive] = colors::Accent;
        }

    } // namespace

    void applyTheme()
    {
        ImGui::StyleColorsDark();
        loadFonts();

        ImGuiStyle& style = ImGui::GetStyle();
        applyStyleMetrics(style);
        applyColorPalette(style);
    }

} // namespace gui