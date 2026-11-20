#include "src/core/FileUtils/osrLoader.h"

#include <fstream>
#include <stdexcept>
#include <string>

std::vector<uint8_t> loadFile(const std::filesystem::path& path)
{
    // binary: bez tego Windows zamienia 0x0D 0x0A i psuje dane
    // ate: kursor od razu na koncu, wiec tellg() zwroci rozmiar pliku
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + path.string());
    }

    const std::streamsize size = file.tellg();
    if (size < 0) {
        throw std::runtime_error("Cannot determine file size: " + path.string());
    }
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(static_cast<size_t>(size));
    if (size > 0 && !file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Cannot read file: " + path.string());
    }

    return buffer;
}