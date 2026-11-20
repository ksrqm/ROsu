#include <cstring>
#include <iostream>

#include "src/tests/FileChecks/FileChecker.h"
#include "src/tests/tests.h"

int main(int argc, char** argv)
{
    if (argc > 1 && std::strcmp(argv[1], "--test") == 0)
    {
        return run_all_tests();
    }

    if (argc < 2)
    {
        std::cout << "Usage: ROsu <replay.osr>\n";
        return 1;
    }

    try
    {
        printReplayInfo(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}