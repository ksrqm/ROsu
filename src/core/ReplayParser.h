#pragma once
#include "../core/BinaryReader.h"
#include "../core/ReplayHeader.h"

ReplayHeader readReplayHeader(BinaryReader& reader);