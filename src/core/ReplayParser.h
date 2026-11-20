#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept> // reporting an error
#include <string>

class Binary
{
public:
    explicit BinaryDecoder(std::vector<uint8_t> data)
        : data_(std::move(data)) {
    }

 
    std::string readString()
    {
        uint8_t indicator = readByte();

        if (indicator == 0x00) {
            return "";
        }
        if (indicator != 0x0b) {
            throw std::runtime_error("Invalid string indicator");
        }

        uint32_t lenght = readULEB128();
        if (lenght > data_.size() - pos_) {
            throw std::out_of_range("Unexpected end of file while reading string");
        }

        std::string value(data_.begin() + pos_, data_.begin() + pos_ + lenght);
        pos_ += lenght;
        return value;

    }

private:
    // vector that holds a 8 bit intiger
    std::vector<uint8_t> data_;
    size_t pos_ = 0;
};