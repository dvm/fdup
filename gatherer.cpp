#include "gatherer.h"
#include "flags.h"

#include <iostream>

namespace fs = std::filesystem;

gatherer const& gatherer::exec(options const& options)
{
    std::error_code ec{};

    for (auto const& directory : options.result()) {
        std::filesystem::recursive_directory_iterator it(directory, fs::directory_options::skip_permission_denied, ec);

        for (fs::directory_entry const& entry : it) {
            if (ec) {
                if (flags::get().verbose_level() > 0) {
                    std::cerr << entry.path() << ": " << "(" << ec.value() << ") " << "[" << ec.category().name() << "] " << ec.message() << "\n";
                }
                continue;
            }

            if (!entry.is_regular_file(ec)) {
                continue;
            }

            if (ec) {
                if (flags::get().verbose_level() > 0) {
                    std::cerr << entry.path() << ": " << "(" << ec.value() << ") " << "[" << ec.category().name() << "] " << ec.message() << "\n";
                }
                continue;
            }

            auto file_size = entry.file_size();
            auto& file_path = entry.path();

            if (file_size == 0 && flags::get().is_omit_zero()) {
                continue;
            }

            _files[file_size].insert(file_path);
        }
    }

    return *this;
}