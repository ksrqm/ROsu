#include <cstring>
#include <iostream>

#include "tests/tests.h"

int main(int argc, char** argv)
{
    // Uruchomienie z argumentem --test odpala testy rdzenia i konczy program.
    if (argc > 1 && std::strcmp(argv[1], "--test") == 0)
    {
        return run_all_tests();
    }

    std::cout << "ROsu\n";
    return 0;
}
