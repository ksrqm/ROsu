#pragma once
#include <cstdint>
#include <string>
#include <vector>


enum class GameMode : uint8_t
{
    Standard = 0,
    Taiko = 1,
    Catch = 2,
    Mania = 3
};


struct ReplayHeader
{
    GameMode mode = GameMode::Standard;

    uint32_t gameVersion = 0;

    std::string beatmapHash;

    std::string playerName;

    std::string replayHash;

    uint16_t count300 = 0;

    uint16_t count100 = 0;

    uint16_t count50 = 0;

    uint16_t countGeki = 0;

    uint16_t countKatu = 0;

    uint16_t countMiss = 0;

    uint32_t totalScore = 0;

    uint16_t maxCombo = 0;

    uint8_t perfectCombo = 0;

    uint32_t modsUsed = 0;

    std::string lifeBar;

    int64_t timeStamp = 0;

    std::vector<uint8_t> compressedData;

    uint64_t scoreID = 0;

    //double modInfo = 0; not yet reading
};