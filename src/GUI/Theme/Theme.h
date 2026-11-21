#pragma once

#include <imgui.h>

namespace gui {

    inline ImFont* RegularFont = nullptr;
    inline ImFont* BoldFont = nullptr;


    // color palette
    namespace colors {
        inline constexpr ImVec4 Background = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
        inline constexpr ImVec4 Surface = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
        inline constexpr ImVec4 SurfaceHover = ImVec4(0.17f, 0.17f, 0.21f, 1.00f);
        inline constexpr ImVec4 Border = ImVec4(0.24f, 0.24f, 0.29f, 1.00f);

        inline constexpr ImVec4 Accent = ImVec4(1.00f, 0.40f, 0.67f, 1.00f); // osu! pink (#FF66AA)
        inline constexpr ImVec4 AccentHover = ImVec4(1.00f, 0.50f, 0.73f, 1.00f);
        inline constexpr ImVec4 AccentActive = ImVec4(0.88f, 0.31f, 0.57f, 1.00f);

        inline constexpr ImVec4 TextPrimary = ImVec4(0.95f, 0.95f, 0.97f, 1.00f);
        inline constexpr ImVec4 TextMuted = ImVec4(0.58f, 0.59f, 0.65f, 1.00f);
        inline constexpr ImVec4 Success = ImVec4(0.30f, 0.85f, 0.55f, 1.00f);
        inline constexpr ImVec4 Error = ImVec4(0.95f, 0.36f, 0.38f, 1.00f);
    }

    void applyTheme();

} // namespace gui