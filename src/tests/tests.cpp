#include "tests.h"
#include "../core/BinaryReader.h"

#include <cassert>
#include <iostream>

// Kazda funkcja testowa sprawdza jedna rzecz.
// Gdy dodasz BinaryReader, testy dopisuj tutaj (np. test_read_int_little_endian).

static void test_sanity()
{
    assert(1 + 1 == 2); // test startowy: potwierdza, ze mechanizm testow dziala
}



static void byte_reading()
{
    BinaryReader reader({ 0x01 });
    //std::cout << static_cast<int>(reader.readByte()) << "\n";
    assert(reader.readByte() == 1);
}

static void int_reading()
{
    BinaryReader reader({ 0x01, 0x00, 0x00, 0x00 });
    assert(reader.readInt() == 1);
}

static void long_reading()
{
    BinaryReader reader({ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
    assert(reader.readLong() == 1);
}

static void ULEB128_reading()
{
    BinaryReader reader({ 0xE5, 0x8E, 0x26 });
    //std::cout << static_cast<uint32_t>(reader.readULEB128()) << "\n";
    assert(reader.readULEB128() == 624485);
}

static void string_reading()
{
    BinaryReader reader({ 0x0b, 0x04, 'o', 's', 'u', '!' });
    //std::cout << reader.readString() << "\n";
    assert(reader.readString() == "osu!");
}

static void bytes_reading()
{
    BinaryReader reader({ 0x01, 0x02 });
    std::vector<uint8_t> bytes = reader.readBytes(2);
    std::vector<uint8_t> expected = { 0x01, 0x02 };

    /*
    for (uint8_t b : bytes) {
        std::cout << static_cast<int>(b) << " ";
    }
    for (uint8_t e : expected) {
        std::cout << static_cast<int>(e) << " ";
    }
    */

    assert(bytes == expected);

}

static void bytes_too_many()
{
    BinaryReader reader({ 0x01, 0x02 });
    reader.readBytes(2);

    bool thrown = false;
    try { reader.readBytes(1); }
    catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
}


int run_all_tests()
{
    // UWAGA: w konfiguracji Release assert() jest wylaczony (NDEBUG),
    // wiec testy uruchamiaj w Debug.
    test_sanity();

    byte_reading();
    int_reading();
    long_reading();
    ULEB128_reading();
    string_reading();
    bytes_reading();
    bytes_too_many();

    std::cout << "All tests passed.\n";
    return 0;
}
