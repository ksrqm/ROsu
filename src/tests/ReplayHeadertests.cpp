// ===== Replay header tests =====
// W tests.cpp dolacz na gorze:
//   #include "../core/ReplayParser.h"
//   #include <string>
//   #include <vector>
//   #include <stdexcept>

// --- pomocnicze: dokladaja bajty w kolejnosci little-endian (odwrotnosc BinaryReader) ---

static void appendByte(std::vector<uint8_t>& v, uint8_t x)
{
    v.push_back(x);
}

static void appendShort(std::vector<uint8_t>& v, uint16_t x)
{
    for (int i = 0; i < 2; i++) {
        v.push_back(static_cast<uint8_t>((x >> (i * 8)) & 0xFF));
    }
}

static void appendInt(std::vector<uint8_t>& v, uint32_t x)
{
    for (int i = 0; i < 4; i++) {
        v.push_back(static_cast<uint8_t>((x >> (i * 8)) & 0xFF));
    }
}

static void appendLong(std::vector<uint8_t>& v, uint64_t x)
{
    for (int i = 0; i < 8; i++) {
        v.push_back(static_cast<uint8_t>((x >> (i * 8)) & 0xFF));
    }
}

// String w formacie osu!: pusty = 0x00, niepusty = 0x0B + dlugosc (ULEB128) + znaki.
// Dla krotkich napisow (< 128 znakow) ULEB128 to jeden bajt.
static void appendString(std::vector<uint8_t>& v, const std::string& s)
{
    if (s.empty()) {
        v.push_back(0x00);
        return;
    }
    v.push_back(0x0B);
    v.push_back(static_cast<uint8_t>(s.size()));
    for (char c : s) {
        v.push_back(static_cast<uint8_t>(c));
    }
}

// Sklada caly naglowek .osr w kolejnosci z pliku.
//  declaredLength - dlugosc zapisana w pliku (mozna podac bledna do testow)
//  data           - faktyczne bajty skompresowanych danych
//  withScoreId    - false = "ucinamy" plik przed polem scoreID
static std::vector<uint8_t> buildReplay(int32_t declaredLength,
    const std::vector<uint8_t>& data,
    bool withScoreId)
{
    std::vector<uint8_t> v;

    appendByte(v, 0);                  // tryb: Standard
    appendInt(v, 20230101);            // wersja gry
    appendString(v, "abc");            // MD5 mapy
    appendString(v, "Filip");          // nick
    appendString(v, "def");            // MD5 powtorki

    appendShort(v, 123);               // n300
    appendShort(v, 45);                // n100
    appendShort(v, 6);                 // n50
    appendShort(v, 7);                 // geki
    appendShort(v, 8);                 // katu
    appendShort(v, 9);                 // miss

    appendInt(v, 1234567);             // wynik
    appendShort(v, 456);               // max combo
    appendByte(v, 1);                  // perfect
    appendInt(v, 72);                  // mody (HD + DT = 8 + 64)
    appendString(v, "");               // wykres zycia
    appendLong(v, 638000000000000000ull); // timestamp

    appendInt(v, static_cast<uint32_t>(declaredLength)); // dlugosc danych
    v.insert(v.end(), data.begin(), data.end());         // dane

    if (withScoreId) {
        appendLong(v, 9876543210ull);  // ID wyniku
    }

    return v;
}

// --- testy ---

static void replay_header_reading()
{
    std::vector<uint8_t> data = { 0xAA, 0xBB, 0xCC };
    BinaryReader reader(buildReplay(3, data, true));

    ReplayHeader h = readReplayHeader(reader);

    assert(h.Mode == GameMode::Standard);
    assert(h.gameVersion == 20230101);
    assert(h.beatmapHash == "abc");
    assert(h.playerName == "Filip");
    assert(h.replayHash == "def");

    assert(h.count300 == 123);
    assert(h.count100 == 45);
    assert(h.count50 == 6);
    assert(h.countGeki == 7);
    assert(h.countKatu == 8);
    assert(h.countMiss == 9);

    assert(h.totalScore == 1234567);
    assert(h.maxCombo == 456);
    assert(h.fullCombo == PerfectCombo::FullCombo);
    assert(h.modsUsed == 72);
    assert(h.lifeBar == "");
    assert(h.timeStamp == 638000000000000000ll);

    assert(h.compressedData == data);
    assert(h.scoreID == 9876543210ull);   // jesli to sie nie zgadza, reader "rozjechal" sie na danych
}

static void replay_header_truncated()
{
    // plik urwany tuz przed scoreID -> ma byc wyjatek, nie smieci
    BinaryReader reader(buildReplay(3, { 0xAA, 0xBB, 0xCC }, false));

    bool thrown = false;
    try { readReplayHeader(reader); }
    catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
}

static void replay_header_negative_length()
{
    BinaryReader reader(buildReplay(-1, {}, true));

    bool thrown = false;
    try { readReplayHeader(reader); }
    catch (const std::runtime_error&) { thrown = true; }
    assert(thrown);
}

static void replay_header_length_too_big()
{
    // plik twierdzi, ze ma 1000 bajtow danych, a ma 3
    BinaryReader reader(buildReplay(1000, { 0xAA, 0xBB, 0xCC }, true));

    bool thrown = false;
    try { readReplayHeader(reader); }
    catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
}

// W run_all_tests() dopisz:
//   replay_header_reading();
//   replay_header_truncated();
//   replay_header_negative_length();
//   replay_header_length_too_big();