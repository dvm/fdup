#pragma once

#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

template <typename T = uint64_t>
class filehasher
{
public:
    typedef T value_type;

    static const value_type INVALID_HASH = UINT64_MAX;

    explicit filehasher(value_type seed = 0)
        : _hash{ seed }
    {
    }

    filehasher* reset(value_type seed = 0)
    {
        _hash = seed;

        return this;
    }

    value_type hash(const std::filesystem::path& file_path, size_t file_size)
    {
        if (std::ifstream is{ file_path, std::ios::binary }) {
            size_t nread{};

            while (is.read(&buffer[0], buffer.size())) {
                nread = is.gcount();
                file_size -= nread;
                update_hash(&buffer[0], nread);
            }

            // process tail
            nread = is.gcount();
            file_size -= nread;
            update_hash(&buffer[0], nread);
        }
        else {
            std::cerr << "Failed to open file: " << file_path.generic_string() << std::endl;
            return INVALID_HASH;
        }

        if (file_size != 0) {
            std::cerr << "Failed to calculate hash of file: " << file_path.generic_string() << std::endl;
            return INVALID_HASH;
        }

        return _hash;
    }

    void update_hash(char* buffer, size_t size)
    {
        std::for_each(buffer, buffer + size,
                      [this](char value) {
                          _hash ^= (_hash << 13) + 27ULL * value;
                      }
        );
    }

private:

    value_type _hash;

    static const size_t BUFFER_SIZE = 64L * 1024;
    std::array<char, BUFFER_SIZE> buffer{};
};
