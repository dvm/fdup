#include "viewer.h"
#include "flags.h"

#include <sstream>

viewer const& viewer::exec(finder const& finder)
{
    for (const auto& [file_hash, file_info] : finder.result()) {
        if (file_info.size() < 2) {
            continue;
        }

        std::cout << std::hex << std::setw(16) << std::setfill('0') << file_hash << std::dec << ": " << file_info.size() << " file(s)";

        std::stringstream ss;
        size_t size{};
        for (const auto& [file_path, file_size] : file_info) {
            ss << "\t\t" << file_path.generic_string() << " (" << file_size << ")" << std::endl;
            size += file_size;
        }

        std::cout << ", size: " << size << " byte(s)\n";
        std::cout << ss.str();
    }

    if (flags::get().is_show_stats()) {
        std::cout << '\n';
        std::cout << "Scanned: " << finder.scanned() << " file(s)\n";
        std::cout << "Duplicated: " << finder.duplicates() << " file(s)\n";
        std::cout << "Duplicated size: " << finder.duplicate_size() << " byte(s)\n";
    }

    return *this;
}
