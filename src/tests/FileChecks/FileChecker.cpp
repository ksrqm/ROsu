#include "FileChecker.h"
#include "src/core/FileUtils/osrLoader.h"
#include "src/core/ReplayParser.h"

#include <iostream>


void printReplayInfo(const std::filesystem::path& path)
{
    BinaryReader reader(loadFile(path));
    ReplayHeader h = readReplayHeader(reader);

    std::cout << "Gamemode:    " << static_cast<int>(h.Mode) << "\n";

    std::cout << "Game Version:    " << h.gameVersion << "\n";

    std::cout << "Beatmap Hash:    " << h.beatmapHash << "\n";

    std::cout << "Player:    " << h.playerName << "\n";

    std::cout << "Replay Hash:    " << h.replayHash << "\n";

    std::cout << "300/100/50/miss: " << h.count300 << "/" << h.count100 << "/" << h.count50 << "\n";

    std::cout << "Geki/Katu/miss: " << h.countGeki << "/" << h.countKatu << "/" << h.countMiss << "\n";

    std::cout << "Total Score:  " << h.totalScore << "\n";

    std::cout << "Max combo: " << h.maxCombo << "\n";

    std::cout << "Perfect combo:  " << static_cast<int>(h.fullCombo) << "\n";

    std::cout << "Mods:      " << h.modsUsed << "\n";
        
    std::cout << "Data size: " << h.compressedData.size() << " bytes\n";
}