#pragma once

#include "src/core/BinaryReader.h"
#include "src/core/ReplayHeader.h"

ReplayHeader readReplayHeader(BinaryReader& reader);