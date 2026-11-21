#include "ReplayInfo.h"
#include "src/GUI/Theme/Theme.h"

#include <imgui.h>
#include <chrono>
#include <cstdio>
#include <string>

namespace gui {

    namespace {

        const char* gameModeToString(GameMode mode)
        {
            switch (mode) {
            case GameMode::Standard: return "osu!standard";
            case GameMode::Taiko:    return "osu!taiko";
            case GameMode::Catch:    return "osu!catch";
            case GameMode::Mania:    return "osu!mania";
            default:                 return "Unknown";
            }
        }

        std::string modsToString(uint32_t mods)
        {
            if (mods == 0) {
                return "None";
            }

            std::string result;
            if (mods & (1 << 0))  result += "NF";
            if (mods & (1 << 1))  result += "EZ";
            if (mods & (1 << 3))  result += "HD";
            if (mods & (1 << 4))  result += "HR";

            // PF (1 << 14)
            if (mods & (1 << 14))      result += "PF";
            else if (mods & (1 << 5))  result += "SD";

            // NC (1 << 9)
            if (mods & (1 << 9))       result += "NC";
            else if (mods & (1 << 6))  result += "DT";

            if (mods & (1 << 7))  result += "RX";
            if (mods & (1 << 8))  result += "HT";
            if (mods & (1 << 10)) result += "FL";
            if (mods & (1 << 12)) result += "SO";

            return result.empty() ? "Other" : "+" + result;
        }

        double calculateAccuracy(const ReplayHeader& h)
        {
            switch (h.Mode) {
            case GameMode::Standard: {
                const double total = static_cast<double>(h.count300 + h.count100 + h.count50 + h.countMiss);
                if (total <= 0.0) return 0.0;
                return (300.0 * h.count300 + 100.0 * h.count100 + 50.0 * h.count50) / (300.0 * total) * 100.0;
            }
            case GameMode::Taiko: {
                const double total = static_cast<double>(h.count300 + h.count100 + h.countMiss);
                if (total <= 0.0) return 0.0;
                return (h.count300 + 0.5 * h.count100) / total * 100.0;
            }
            case GameMode::Catch: {
                const double caught = static_cast<double>(h.count300 + h.count100 + h.count50);
                const double total = caught + static_cast<double>(h.countKatu + h.countMiss);
                if (total <= 0.0) return 0.0;
                return (caught / total) * 100.0;
            }
            case GameMode::Mania: {
                const double total = static_cast<double>(
                    h.countGeki + h.count300 + h.countKatu + h.count100 + h.count50 + h.countMiss
                    );
                if (total <= 0.0) return 0.0;
                const double weighted = 300.0 * (h.countGeki + h.count300)
                    + 200.0 * h.countKatu
                    + 100.0 * h.count100
                    + 50.0 * h.count50;
                return weighted / (300.0 * total) * 100.0;
            }
            default:
                return 0.0;
            }
        }

        std::string formatTimestamp(int64_t ticks)
        {
            constexpr int64_t unixEpochTicks = 621355968000000000LL;
            if (ticks <= unixEpochTicks) {
                return "Unknown";
            }

            const auto seconds = std::chrono::seconds((ticks - unixEpochTicks) / 10000000LL);
            const auto sysTime = std::chrono::sys_seconds(seconds);
            const auto days = std::chrono::floor<std::chrono::days>(sysTime);

            const std::chrono::year_month_day ymd{ days };
            const std::chrono::hh_mm_ss hms{ sysTime - days };

            char buffer[32];
            std::snprintf(
                buffer,
                sizeof(buffer),
                "%04d-%02u-%02u %02lld:%02lld UTC",
                static_cast<int>(ymd.year()),
                static_cast<unsigned>(ymd.month()),
                static_cast<unsigned>(ymd.day()),
                static_cast<long long>(hms.hours().count()),
                static_cast<long long>(hms.minutes().count())
            );
            return buffer;
        }

        void drawStatRow(const char* label, const char* value, const ImVec4* valueColor = nullptr)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextDisabled("%s", label);

            ImGui::TableSetColumnIndex(1);
            if (valueColor != nullptr) {
                ImGui::TextColored(*valueColor, "%s", value);
            }
            else {
                ImGui::Text("%s", value);
            }
        }

    } // namespace

    void renderReplayInfo(const std::optional<ReplayHeader>& header)
    {
        if (!header.has_value()) {
            return;
        }

        const ReplayHeader& h = *header;

        ImGuiChildFlags childFlags = ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY;
        ImGui::BeginChild("ReplayInfoCard", ImVec2(-1.0f, 0.0f), childFlags);

        // GameMode and Player
        if (BoldFont != nullptr) ImGui::PushFont(BoldFont);
        ImGui::TextColored(colors::Accent, "%s", h.playerName.empty() ? "Unknown Player" : h.playerName.c_str());
        if (BoldFont != nullptr) ImGui::PopFont();

        ImGui::SameLine();
        ImGui::TextDisabled("(%s)", gameModeToString(h.Mode));

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::BeginTable("ReplayStatsTable", 2, ImGuiTableFlags_SizingStretchProp)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 105.0f);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

            // Score
            char scoreBuf[32];
            std::snprintf(scoreBuf, sizeof(scoreBuf), "%u", h.totalScore);
            drawStatRow("Score", scoreBuf);

            // Accuracy
            char accBuf[32];
            std::snprintf(accBuf, sizeof(accBuf), "%.2f%%", calculateAccuracy(h));
            drawStatRow("Accuracy", accBuf);

            // Max Combo + status FC
            const bool isFc = (h.fullCombo == PerfectCombo::FullCombo && h.countMiss == 0);
            char comboBuf[48];
            std::snprintf(comboBuf, sizeof(comboBuf), "%ux%s", h.maxCombo, isFc ? "  [FC]" : "");
            drawStatRow("Max Combo", comboBuf, isFc ? &colors::Success : nullptr);

            // Mods
            const std::string modsStr = modsToString(h.modsUsed);
            drawStatRow("Mods", modsStr.c_str());

            // Hit counts (300 / 100 / 50 / Miss)
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextDisabled("Hits");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u / %u / %u / ", h.count300, h.count100, h.count50);
            ImGui::SameLine(0.0f, 0.0f);
            if (h.countMiss > 0) {
                ImGui::TextColored(colors::Error, "%u", h.countMiss);
            }
            else {
                ImGui::TextColored(colors::Success, "0");
            }

            // Data
            const std::string dateStr = formatTimestamp(h.timeStamp);
            drawStatRow("Date", dateStr.c_str());

            // MD5 beatmap
            const std::string shortHash = h.beatmapHash.size() > 12
                ? h.beatmapHash.substr(0, 12) + "..."
                : h.beatmapHash;
            drawStatRow("Beatmap MD5", shortHash.c_str(), &colors::TextMuted);

            ImGui::EndTable();
        }

        ImGui::EndChild();
    }

} // namespace gui