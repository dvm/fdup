#include <filesystem>
#include <fstream>
#include <chrono>
#include <map>
#include <set>
#include <array>
#include <cassert>
#include <iostream>

#include <openssl/md5.h>

namespace fs = std::filesystem;

const std::size_t kChunkSize{4096};

bool verbose{false};

struct file_hash
{
    typedef std::array<unsigned char, MD5_DIGEST_LENGTH> value_type;
    value_type value;

    file_hash() noexcept = default;

    explicit file_hash(const std::string& file_path)
    {
        assert(!file_path.empty());
        calc_hash(file_path);
    }

    void calc_hash(const std::string& file_path)
    {
        if (std::ifstream is{file_path, std::ios::binary | std::ios::ate}) {
            auto file_size = is.tellg();
            is.seekg(0);

            std::array<char, kChunkSize> chunk{};

            MD5_CTX c;
            MD5_Init(&c);

            while (auto size = is.readsome(&chunk[0], kChunkSize)) {
                if (size < 0)
                    break;

                MD5_Update(&c, &chunk[0], size);
                file_size -= size;
            }

            MD5_Final(&value[0], &c);

            assert(file_size == 0);
        }
    }
};

bool operator<(const file_hash& lhs, const file_hash& rhs)
{
    return lhs.value < rhs.value;
}

bool operator==(const file_hash& lhs, const file_hash& rhs)
{
    return lhs.value == rhs.value;
}

std::ostream& operator<<(std::ostream& os, const file_hash& rhs)
{
    os << std::hex;
    for (std::size_t i = 0; i < rhs.value.size(); ++i)
        std::cout << std::setw(2) << std::setfill('0') << (int)rhs.value[i];
    return os << std::dec;
}

using file_size_t = uintmax_t;
using file_path_t = std::string;

std::multimap<file_size_t, file_path_t> files;
std::multimap<file_hash, const file_path_t*> duplicates;

file_size_t duplicate_size{};
std::size_t duplicate_files{};

void print_duplicates(const file_hash& hash)
{
    auto [begin, end] = duplicates.equal_range(hash);

    if (++begin == end)
        return;

    std::cout << hash << ":\n";

    for (auto it = --begin; it != end; ++it) {
        std::cout << '\t' << *it->second << '\n';
        ++duplicate_files;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [PATH]..." << std::endl;
        return EXIT_FAILURE;
    }

    std::set<fs::path> directories;

    --argc;
    for ( ;argc > 0; --argc) {
        if (argv[argc][0] == '-' && argv[argc][1] == 'v') {
            verbose = true;
        }
        else {
            fs::path path{argv[argc]};

            if (fs::exists(path) && fs::is_directory(path)) {
                directories.insert(path);
            }
        }
    }

    auto start = std::chrono::steady_clock::now();

    for (const auto& path : directories) {
        for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(path)) {
            if (dir_entry.is_regular_file()) {
                files.emplace(dir_entry.file_size(), dir_entry.path());
            }
        }
    }

    for (const auto& [size, file_path] : files) {
        if (fs::is_symlink(file_path))
            continue;
        if (files.count(size) > 1) {
            duplicates.emplace(file_hash(file_path), &file_path);
            duplicate_size += size;
        }
    }

    auto it = duplicates.begin();
    auto end = duplicates.end();
    while (it != end) {
        const auto& hash = it->first;

        print_duplicates(hash);

        do {
            if (++it == end)
                break;
        } while (hash == it->first);
    }

    std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - start;

    if (verbose) {
        std::cout << '\n';
        std::cout << "Scanned: " << files.size() << " files\n";
        std::cout << "Duplicates: " << duplicate_files << " files\n";
        std::cout << "Duplicate size: " << duplicate_size << " bytes\n";
        std::cout.precision(3);
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
    }

    return 0;
}
