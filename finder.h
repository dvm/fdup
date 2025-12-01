#pragma once

#include "filehasher.h"
#include "gatherer.h"

#include <unordered_map>
#include <vector>

class finder
{
public:

    typedef std::tuple<std::filesystem::path, size_t> fileinfo_t;
    using result_type = std::unordered_map<filehasher<>::value_type, std::vector<fileinfo_t>>;

    finder() = default;

    finder const& exec(gatherer const& gatherer);

    result_type const& result() const
    {
        return _hashes;
    }

    finder const& reset()
    {
        _hashes.clear();

        return *this;
    }

    size_t scanned() const
    {
        return _scanned_files;
    }

    size_t duplicates() const
    {
        return _duplicates;
    }

    size_t duplicate_size() const
    {
        return _duplicate_size;
    }

private:

    result_type _hashes{};

    size_t _scanned_files{};
    size_t _duplicates{};
    size_t _duplicate_size{};
};
