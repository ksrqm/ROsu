#include "../core/ReplayParser.h"
#include <vector>

ReplayHeader readReplayHeader(BinaryReader& reader)
{
	ReplayHeader h;

	h.Mode = static_cast<GameMode>(reader.readByte());

	h.gameVersion = static_cast<uint32_t>(reader.readInt());

	h.beatmapHash = reader.readString();

	h.playerName = reader.readString();

	h.replayHash = reader.readString();

	h.count300 = static_cast<uint16_t>(reader.readShort());
	h.count100 = static_cast<uint16_t>(reader.readShort());
	h.count50 = static_cast<uint16_t>(reader.readShort());

	h.countGeki = static_cast<uint16_t>(reader.readShort());
	h.countKatu = static_cast<uint16_t>(reader.readShort());
	h.countMiss = static_cast<uint16_t>(reader.readShort());

	h.totalScore = static_cast<uint32_t>(reader.readInt());

	h.maxCombo = static_cast<uint16_t>(reader.readShort());
	h.fullCombo = (reader.readByte() == 1) ? PerfectCombo::FullCombo : PerfectCombo::NotFullCombo;

	h.modsUsed = static_cast<uint32_t>(reader.readInt());
	
	h.lifeBar = reader.readString();

	h.timeStamp = static_cast<int64_t>(reader.readLong());

	int32_t CompressedLenght = reader.readInt();
	if (CompressedLenght > 0) {
		h.compressedData = std::vector<uint8_t>(CompressedLenght);
	}

	h.scoreID = static_cast<uint64_t>(reader.readLong());

	return h;
}