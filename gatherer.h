#pragma once

#include "options.h"

#include <set>
#include <unordered_map>
#include <filesystem>

class gatherer
{
public:

    typedef std::unordered_map<size_t, std::set<std::filesystem::path>> result_type;

    gatherer() = default;

    gatherer const& exec(options const& options);

    result_type const& result() const
    {
        return _files;
    }

    gatherer const& reset()
    {
        _files.clear();

        return *this;
    }

private:

    result_type _files{};
};
