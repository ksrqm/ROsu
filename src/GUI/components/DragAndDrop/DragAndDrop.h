#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace gui {

    std::optional<std::filesystem::path> renderDropZone(
        const std::optional<std::filesystem::path>& currentFile,
        const std::string& errorMessage
    );
}