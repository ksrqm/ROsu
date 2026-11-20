#include "FileChecker.h"
#include "src/core/FileUtils/osrLoader.h"
#include "src/core/ReplayParser.h"

#include <iostream>


void printReplayInfo(const std::filesystem::path& path)
{
    BinaryReader reader(loadFile(path));
    ReplayHeader h = readReplayHeader(reader);

    std::cout << "Player:    " << h.playerName << "\n";

}