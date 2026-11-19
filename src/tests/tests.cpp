#include "tests.h"

#include <cassert>
#include <iostream>

// Kazda funkcja testowa sprawdza jedna rzecz.
// Gdy dodasz BinaryReader, testy dopisuj tutaj (np. test_read_int_little_endian).

static void test_sanity()
{
    assert(1 + 1 == 2); // test startowy: potwierdza, ze mechanizm testow dziala
}

int run_all_tests()
{
    // UWAGA: w konfiguracji Release assert() jest wylaczony (NDEBUG),
    // wiec testy uruchamiaj w Debug.
    test_sanity();

    std::cout << "Wszystkie testy przeszly.\n";
    return 0;
}
