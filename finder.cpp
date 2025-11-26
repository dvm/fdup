#include "finder.h"

finder const& finder::exec(gatherer const& gatherer)
{
    auto h = std::make_unique<filehasher<>>();

    for (const auto& [file_size, pathes] : gatherer.result()) {
        if (pathes.size() < 2) {
            continue;
        }

        for (const auto& file_path : pathes) {
            auto file_hash = h->reset()->hash(file_path, file_size);

            if (file_hash == filehasher<>::INVALID_HASH) {
                continue;
            }

            _hashes[file_hash].emplace_back(file_path, file_size);

            ++_duplicates;
            _duplicate_size += file_size;
        }
    }

    _scanned_files = gatherer.result().size();

    return *this;
}
