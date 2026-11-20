#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept> // reporting an error
#include <string>

class BinaryReader
{
public:
    explicit BinaryReader(std::vector<uint8_t> data)
        : data_(std::move(data)) {
    }

    uint8_t readByte()
    {
        //check if there is a byte to read
        if (pos_ >= data_.size()){
            throw std::out_of_range("End of file");
        }

        //return byte
        return data_[pos_++];
    }

    int16_t readShort()
    {
        uint16_t value = 0;

        for (int i = 0; i < 2; i++) {
            value |= static_cast<uint16_t>(readByte()) << (i * 8);
        }

        return static_cast<int16_t>(value);
    }

    int32_t readInt()
    {
        uint32_t value = 0;

        //read byte 4 times with moving cursor by 8 bits every time
        for (int i = 0; i < 4; ++i) {
            value |= static_cast<uint32_t>(readByte()) << (i * 8);
        }

        return static_cast<int32_t>(value);
    }

    int64_t readLong()
    {
        uint64_t value = 0;

        for (int i = 0; i < 8; i++) {
            value |= static_cast<uint64_t>(readByte()) << (i * 8);
        }

        return static_cast<int64_t>(value);
    }

    uint32_t readULEB128()
    {
        uint32_t value = 0;
        uint32_t shift = 0;

        while (true) {
            uint8_t byte = readByte();
            value |= static_cast<uint32_t>(byte & 0x7F) << shift;

            if ((byte & 0x80) == 0) {
                break;
            }

            shift += 7;
            if (shift >= 35) {
                throw std::runtime_error("Corrupted ULEB128 value");
            }
        }

        return value;
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