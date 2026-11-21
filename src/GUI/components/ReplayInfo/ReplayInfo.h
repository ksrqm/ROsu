#pragma once

#include "src/core/ReplayHeader.h"
#include <optional>

namespace gui {

    void renderReplayInfo(const std::optional<ReplayHeader>& header);

}