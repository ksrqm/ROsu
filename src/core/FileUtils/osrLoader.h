#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>

std::vector<uint8_t> loadFile(const std::filesystem::path& path);