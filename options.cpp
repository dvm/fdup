#include "flags.h"
#include "options.h"

#include <iostream>

namespace fs = std::filesystem;

options::options(int argc, char** argv)
    : _argc{ argc }
    , _argv{ argv }
    , _progname{ fs::path{argv[0]}.string() }
{
}

void options::exit_usage(int exit_code) const
{
    std::cerr << "Usage: " << _progname << " [-s] [-o] [-t] [-v] PATH...\n\n"
              << "\t-s\t\tshow statistics\n"
              << "\t-o\t\tomit zero-length files\n"
              << "\t-t\t\tuse relative pathes for scan files\n"
              << "\t-v\t\tverbose output, including statistics" << std::endl;

    exit(exit_code);
}

void options::parse()
{
    if (_argc < 2) {
        exit_usage(EXIT_FAILURE);
    }

    --_argc;
    for (int i = _argc; i > 0; --i) {
        if (_argv[i][0] == '-') {
            switch (_argv[i][1]) {
            case 'v':
                ++flags::get()._verbose;
                break;
            case 's':
                flags::get()._show_stats = true;
                break;
            case 'o':
                flags::get()._omit_zero = true;
                break;
            case 't':
                flags::get()._relative_path = true;
                break;
            default:
                std::cerr << "Unknown option: " << _argv[i] << std::endl;
                exit_usage(EXIT_FAILURE);
            }
        }
    }

    for (int i = _argc; i > 0; --i) {
        if (_argv[i][0] != '\0' &&
            _argv[i][0] != '-')
        {
            fs::path path{ _argv[i] };

            if (fs::exists(path) && fs::is_directory(path)) {
                _directories.insert(flags::get()._relative_path ? path : fs::absolute(path));
            }
            else {
                std::cerr << path.string() << ": Directory doesn't exists." << std::endl;
                exit_usage(EXIT_FAILURE);
            }
        }
    }

    if (_directories.empty())
    {
        std::cerr << "No directories for scan." << std::endl;
        exit_usage(EXIT_FAILURE);
    }
}
