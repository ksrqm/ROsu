#include <cstring>
#include <iostream>

#include "src/GUI/AppWindow.h"
#include "src/tests/FileChecks/FileChecker.h"
#include "src/tests/tests.h"

int main(int argc, char** argv)
{
    try
    {
        // ROsu --test        -> core tests
        if (argc > 1 && std::strcmp(argv[1], "--test") == 0)
        {
            return run_all_tests();
        }

        // ROsu <file.osr>
        if (argc > 1)
        {
            printReplayInfo(argv[1]);
            return 0;
        }

        // ROsu               -> GUI
        AppWindow window(1280, 720, "ROsu");
        window.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}